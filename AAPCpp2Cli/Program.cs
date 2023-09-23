using System;
using System.Text;
using System.Text.RegularExpressions;

class Program
{
    private static Regex reCls = new(@"^ *(class|struct) ([A-z_0-9 ]+)\W*(\{(?:[^{}]|(?<open>\{)|(?<-open>\}))+(?(open)(?!))\})", RegexOptions.Multiline);
    private static Regex reCom = new(@"/\*\w+\*/");
    private static Regex reDeclDef = new(@"^([A-z_0-9 :<,>]+)\(([A-z_0-9\- ,=:<>&*/.\r\n]*)\)\s*((\{(?:[^{}]|(?<open>\{)|(?<-open>\}))+(?(open)(?!))\})|;)", RegexOptions.Multiline);
    private static Regex reImpl = new(@"^([A-z_0-9 :<,>]+)\(([A-z_0-9 ,=:<>&*/.\r\n]*)\)[\sa-z]*((:([\s ]*[A-z_0-9]+\{[A-z0-9.]+\},)+[\s ]*[A-z_0-9]+\{[A-z0-9.]+\})?\s*(\{(?:[^{}]|(?<open>\{)|(?<-open>\}))+(?(open)(?!))\}))", RegexOptions.Multiline);
    private static Regex reMemb = new(@"^([A-z_0-9 :<,>]+)(\{[A-z0-9.:]+\})?;", RegexOptions.Multiline);
    private static Regex reVector = new(@"std::vector<([^,<>]*)>");
    private static Regex reListInit = new(@"(System::Collections::Generic::List<[A-z0-9_]+>)\^\s*(\w+);");
    private static Regex reLambda = new(@"\[\]\((.*)\)\s*->\s*([A-z]+)\s*(\{(?:[^{}]|(?<open>\{)|(?<-open>\}))+(?(open)(?!))\})");
    private static Regex reInclude = new(@"^#include .*$", RegexOptions.Multiline);
    private static Regex reEndif = new(@"^#endif .*$", RegexOptions.Multiline);

    private static string Namespace = "AAPlusSharp";

    public static int Main(string[] args)
    {
        if (args.Length != 2)
            return 1;

        Console.WriteLine("currently no AAVSOP2013");

        var input = args[0];
        var output = args[1];

        Directory.CreateDirectory(output);
        var exclusion = new string[] { "AA+.h", "stdafx.h", "stdafx.cpp" };

        foreach (var p in Directory.EnumerateFiles(input, "*.h"))
        {
            using var sr = new StreamReader(p);
            using var sw = new StreamWriter(Path.Combine(output, Path.GetFileName(p)));
            if (exclusion.Any(e => p.EndsWith(e)))
                sw.Write(sr.ReadToEnd());
            else
                sw.Write(ProcessHeader(sr.ReadToEnd()));
        }

        foreach (var p in Directory.EnumerateFiles(input, "*.cpp"))
        {
            using var sr = new StreamReader(p);
            using var sw = new StreamWriter(Path.Combine(output, Path.GetFileName(p)));
            if (exclusion.Any(e => p.EndsWith(e)))
                sw.Write(sr.ReadToEnd());
            else
                sw.Write(ProcessImpl(sr.ReadToEnd()));
        }

        return 0;
    }

    private static string RemoveQualifiers(string cpp)
    {
        var cli = new StringBuilder(cpp);
        cli.Replace(") noexcept", ")");
        cli.Replace(") const noexcept", ")");
        cli.Replace("constexpr ", "");
        cli.Replace("const ", ""); // !!!
        cli.Replace(" const", ""); // !!!
        return cli.ToString();
    }

    private static string ReplaceStd(string cpp)
    {
        //var cli = new StringBuilder(cpp);
        //cli.Replace("#include <vector>", "#include \"AAPArray.h\"");
        //cli.Replace("std::vector", "AAPArray");
        //return cli.ToString();

        //cpp = cpp.Replace("#include <vector>", "#include \"AAPArray.h\"");
        cpp = cpp.Replace("#include <vector>", "");
        //cpp = reVector.Replace(cpp, "AAPArray<$1>");
        cpp = reVector.Replace(cpp, "System::Collections::Generic::List<$1>^");
        cpp = cpp.Replace(".push_back", "->Add");
        cpp = reListInit.Replace(cpp, "$1^ $2 = gcnew $1(0);");
        return cpp;
    }

    public static string ProcessHeader(string cpp)
    {
        cpp = RemoveQualifiers(cpp);
        cpp = ReplaceStd(cpp);

        var cli = new StringBuilder(cpp);
        var matches = reCls.Matches(cpp);
        foreach (Match m in matches.Reverse<Match>())
        {
            var name = m.Groups[2].Value;
            if (name.Contains("Coefficient"))
            {
                // native type, do nothing
            }
            else
            {
                var type = m.Groups[1].Value;
                //var isRef = type == "class";
                //var isRef = !name.Contains("Details2");
                var body = ProcessHeaderClassBody(out var isRef, name, m.Groups[3].Value);
                var rv = isRef ? "ref" : "value";
                var cls = $"public {rv} {type} {name}\r\n{body}";
                cli.Remove(m.Index, m.Length);
                cli.Insert(m.Index, cls);
            }
        }
        cpp = cli.ToString();

        cpp = EdgeCases(cpp);
        if (matches.Count > 0)
            cpp = AddNamespace(cpp, matches[0].Index, reEndif.Matches(cpp).Last().Index);

        return cpp;
    }

    public static string ProcessHeaderClassBody(out bool isRef, string name, string body)
    {
        int numMethods = reDeclDef.Matches(body).Count;
        body = FixHeaderOverload(body, reDeclDef.Matches(body));
        body = FixHeaderRefPointer(body, reDeclDef.Matches(body));

        // pure data struct, require copy constructor and operator=
        //if (isRef && numMethods == 0)
        //{
        //    var co = GenConstructorAndOperator(name, body);
        //    var bd = new StringBuilder(body);
        //    bd.Insert(body.Length - 1, co);
        //    body = bd.ToString();
        //}
        isRef = numMethods > 0;
        if (!isRef)
        {
            body = reMemb.Replace(body, "$1;");
        }

        return body;
    }

    private static string FixHeaderRefPointer(string body, MatchCollection matches)
    {
        var bd = new StringBuilder(body);
        foreach (var m in matches.Reverse())
        {
            var param = m.Groups[2].Value;
            param = reCom.Replace(param, string.Empty);
            param = param.Replace('&', '%');
            //param = param.Replace('*', '^');
            bd.Remove(m.Groups[2].Index, m.Groups[2].Length);
            bd.Insert(m.Groups[2].Index, param);
        }
        body = bd.ToString();
        return body;
    }

    private static string FixHeaderOverload(string body, MatchCollection matches)
    {
        var bd = new StringBuilder(body);
        foreach (var m in matches.Reverse())
        {
            var param = m.Groups[2].Value;
            if (param.Contains('='))
            {
                var ol = GenOverloads(m);
                bd.Remove(m.Index, m.Length);
                bd.Insert(m.Index, ol);
            }
        }
        body = bd.ToString();
        return body;
    }

    public static string GenOverloads(Match m)
    {
        var ol = new StringBuilder();
        var qualName = m.Groups[1].Value;
        var sptOpt = StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries;
        var name = qualName.Split(' ', sptOpt).Last();
        var paramList = m.Groups[2].Value.Split(',', sptOpt);
        var normParams = paramList.TakeWhile(p => !p.Contains('=')).ToArray();
        var normParamNames = normParams.Select(p => p.Split(' ', sptOpt).Last());
        var defParams = paramList.SkipWhile(p => !p.Contains('=')).ToArray();
        var defParamTypeNameValues = defParams.Select(p => p.Split('=', sptOpt)).ToArray();
        var defParamTpyeNames = defParamTypeNameValues.Select(tnv => tnv[0]);
        var defParamNames = defParamTpyeNames.Select(tn => tn.Split(' ', sptOpt).Last());
        var defParamValues = defParamTypeNameValues.Select(tnv => tnv[1]);
        var body = m.Groups[3].Value;
        ol.Append(qualName);
        ol.Append('(');
        ol.Append(string.Join(", ", normParams.Concat(defParamTpyeNames)));
        ol.Append(')');
        ol.AppendLine(body);
        for (var i = 0; i < defParams.Length; i++)
        {
            ol.Append(qualName);
            ol.Append('(');
            ol.Append(string.Join(", ", normParams.Concat(defParamTpyeNames.Take(i))));
            ol.Append(") { return ");
            ol.Append(name);
            ol.Append('(');
            var actualParam = normParamNames.Concat(defParamNames.Take(i))
                                            .Concat(defParamValues.TakeLast(defParams.Length - i));
            ol.Append(string.Join(", ", actualParam));
            ol.AppendLine("); }");
        }
        return ol.ToString();
    }

    public static string GenConstructorAndOperator(string name, string body)
    {
        var sptOpt = StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries;

        var membs = reMemb.Matches(body);
        var membNames = membs.Select(m => m.Groups[1].Value.Split(' ', sptOpt).Last()).ToArray();
        var assign = string.Join("\r\n", membNames.Select(n => $"    {n} = other.{n};"));

        name = name.Split(' ', sptOpt).Last();
        var co = new StringBuilder();
        co.AppendLine("public:");
        co.AppendLine($"  {name}() {{}}");
        co.AppendLine($"  {name}({name}% other)\r\n  {{");
        co.AppendLine(assign);
        co.AppendLine("  }");
        //co.AppendLine($"  {name}% operator=({name} other)\r\n  {{");
        //co.AppendLine(assign);
        //co.AppendLine("    return *this;");
        //co.AppendLine("  }");
        //co.AppendLine($"  {name}% operator=({name}% other)\r\n  {{");
        //co.AppendLine(assign);
        //co.AppendLine("    return *this;");
        //co.AppendLine("  }");
        co.AppendLine($"  void operator=({name}% other)\r\n  {{");
        co.AppendLine(assign);
        co.AppendLine("  }");
        return co.ToString();
    }

    public static string ProcessImpl(string cpp)
    {
        cpp = RemoveQualifiers(cpp);
        cpp = ReplaceStd(cpp);

        var cli = new StringBuilder(cpp);
        var lambdas = new Dictionary<string, string>();
        var matches = reImpl.Matches(cpp);
        foreach (var m in matches.Reverse())
        {
            var impl = ProcessImplBody(m.Groups[1].Value, m.Groups[2].Value, m.Groups[3].Value, ref lambdas);
            cli.Remove(m.Index, m.Length);
            cli.Insert(m.Index, impl);
        }
        if (lambdas.Count > 0)
        {
            var lds = String.Join("\r\n", lambdas.Values);
            cli.Insert(matches[0].Index, lds);
        }
        cpp = cli.ToString();

        cpp = EdgeCases(cpp);

        cli = new StringBuilder(cpp);
        var lastInclude = reInclude.Matches(cpp).Last();
        cli.Insert(lastInclude.Index + lastInclude.Length + 1, $"using namespace {Namespace};");
        cpp = cli.ToString();
        return cpp;
    }

    public static string ProcessImplBody(string qualName, string paramList, string initBody, ref Dictionary<string, string> lambdas)
    {
        paramList = reCom.Replace(paramList, string.Empty);
        paramList = paramList.Replace('&', '%');
        //paramList = paramList.Replace('*', '^');

        foreach (var m in reLambda.Matches(initBody).Reverse())
        {
            var li = lambdas.Count;
            var name = $"_CliGenLambda{li}";
            var pl = m.Groups[1].Value;
            pl = $"const {pl.Replace(",", ", const ")}"; // A temp fix
            lambdas[name] = $"{m.Groups[2].Value} {name}({pl}){m.Groups[3].Value}\r\n";
            var ib = new StringBuilder(initBody);
            ib.Remove(m.Index, m.Length);
            ib.Insert(m.Index, name);
            initBody = ib.ToString();
        }

        var impl = new StringBuilder();
        impl.Append(qualName);
        impl.Append('(');
        impl.Append(paramList);
        impl.Append(")");
        if (initBody[0] == ':')
            impl.Append(' ');
        else
            impl.AppendLine();
        impl.Append(initBody);
        return impl.ToString();
    }

    private static string AddNamespace(string cpp, int beg, int end)
    {
        var cli = new StringBuilder(cpp);
        cli.Insert(end, "}\r\n");
        cli.Insert(beg, $"namespace {Namespace}\r\n{{\r\n");
        return cli.ToString();
    }

    private static string EdgeCases(string cpp)
    {
        // AAJewishCalendar.cpp L80,83
        //cpp = cpp.Replace("const CAADate CurrentYear{CivilYear, CurrentPesach.Month, static_cast<double>(CurrentPesach.Day), bGregorian};",
        //                  "CAADate CurrentYear{CivilYear, CurrentPesach.Month, static_cast<double>(CurrentPesach.Day), bGregorian};");
        //cpp = cpp.Replace("const CAADate NextYear{CivilYear+1, NextPesach.Month, static_cast<double>(NextPesach.Day), bGregorian};",
        //                  "CAADate NextYear{CivilYear+1, NextPesach.Month, static_cast<double>(NextPesach.Day), bGregorian};");

        // AADynamicalTime.h, L67
        cpp = cpp.Replace("static DELTAT_PROC sm_pDeltaTProc;",
                          "static DELTAT_PROC sm_pDeltaTProc{ nullptr };");
        // AADynamicalTime.cpp, L19771
        cpp = cpp.Replace("CAADynamicalTime::DELTAT_PROC CAADynamicalTime::sm_pDeltaTProc{nullptr};",
                          "//CAADynamicalTime::DELTAT_PROC CAADynamicalTime::sm_pDeltaTProc{nullptr};");

        // AADynamicalTime.cpp, L19813
        //cpp = cpp.Replace("const CAADate date{JD, CAADate::AfterPapalReform(JD)};",
        //                  "CAADate date{JD, CAADate::AfterPapalReform(JD)};");

        // ...
        cpp = cpp.Replace("CAA3DCoordinate*", "CAA3DCoordinate^");
        cpp = cpp.Replace("CAA3DCoordinate Ecliptic{EclipticRectangularCoordinates(pT, nTSize, correction, &EclipticDerivative)};",
                          "CAA3DCoordinate Ecliptic{EclipticRectangularCoordinates(pT, nTSize, correction, %EclipticDerivative)};");

        // AAELP2000.cpp, L107 && AAELPMPP02.cpp, L252
        cpp = cpp.Replace("std::array<std::array<double, 2>, 8> g_P",
                          "static std::array<std::array<double, 2>, 8> g_P");

        return cpp;
    }
}