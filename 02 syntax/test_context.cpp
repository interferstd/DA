#if __cplusplus < 201103L
# include <bits/c++0x_warning.h>
#else
#include "context.hpp"
#include <time.h>

using namespace std;

int main()
{
	context::context cntxt;
	cntxt.in() = fopen("test_np.da","rb");
	cntxt.out() = stdout;

	try{
		AVLHML<string,char> types;
		types["int"],types["char"],types["long"],types["bool"],types["float"];
		cntxt
			// Replace standart interrupt
//			("\1",[&](){})
			("start",[&](){printf("[start work]\n");})
			("end",[&](){printf("\n[end work]");})
			("\1 ",[](){})
			("is type",[&](){cntxt.control_obj.flag() = types.search(cntxt.control_obj.last().content); })
//            ("",[&](){ cntxt.control_obj.massage()})
			// End of declaratio interrupts
			// Start interrupt
			("Start","pg{start}<program>{end}u")
			// Basic interrupts
			("program","<neof>w(<dbg \n><skip \n><dbg out><declarations>g)b")
			("operator","<if>i<while>i<for>i<switch>i<{[operators]}>i<expression>w'uncorrect operator't")
			("if","'if'qnirg<skip \n>'('qnw'expected \"(\"'tg<expression>'('qnw'expected \")\"'tg<skip \n><operator><skip \n>'else'qnirg<operator><skip \n>cf")
			("while","'while'qnirg<skip \n>'('qnw'expected \"(\"'tg<expression>'('qnw'expected \")\"'tg<skip \n><operator><skip \n>'else'qnirg<operator><skip \n>cf")
			("for","'for'qnirg<skip \n>'('qnw'expected \"(\"'tg<body for>'('qnw'expected \")\"'tg<skip \n><operator><skip \n>'else'qnirg<operator><skip \n>cf")
			("body for","<operator>';'qnw'expected \";\"'tg<operator>';'qnw'expected \";\"'tg<operator>")
			("switch","")// ("switch","'switch'qnirg<skip \n>'('qnw'expected \"(\"'tg<expression>'('qnw'expected \")\"'{'")
			("{[operators]}","'{'qnirg<skip \n>(<eof>'}'on)w(<operator><skip \n;>)b<eof>w'finde eof, expected \"}\"'t'}'qwgrcf")
			("expression",
				"<goto[label]>i<:[lable]>i<::=[operator]>i<expression 15>w'uncorrect expression't")
			("goto[label]","'goto'qnirgNqnw'expected lable identity't")
			(":[lable]","':'qnirgNqnw'expected defenition lable identity't")
			("::=[operator]","'::='qnirg<operator>cf")
			("declarations","<declaration var>i<declaration func>w'uncorrect declaration'")
			("declaration var","{is type}nirg('*'q)w(g)b<name>g(','q)w(g<name>g)b';'q'\n'[0]onw'ecxpected \"\\n\" or \";\"'tg")
			("declaration func","<name>g<skip \n>'::='qnw'expected ::='tg<skip \n><description types><description names><description branches>")
			("description types", "(<type>ir(','q)w(g<skip \n><type>g)b)<skip \n>('->'qnirg<type>)<skip \n>")
			("description names", "'('qnir(g<skip \n>Nqnirg(','q)w(g<skip \n><name>g)b)<skip \n>')'qnw'expected )'tg<skip \n>")
			("description branches", "(<operator>';'q'\n'[0]owgr'excepted ; or \\n't)('('q)w(<expression 14>(','q)w(<expression 14>)b')'qnw'expected )'t<operator>';'q'\n'[0]onw'ecxpected \"\\n\" or \";\"'tg)b")
			("name","Nqnw'excepted identity't")
			("type", "{is type}nirg('*'q)w(g)bcf")

			// expression
				("expression 15","<expression 14><priority 15>w(g<expression 14>)b")
				("expression 14","<expression 13><priority 14>w(g<expression 13>)b")
				("expression 13","<expression 12><priority 13>w(g<expression 12>)b")
				("expression 12","<expression 11><priority 12>w(g<expression 11>)b")
				("expression 11","<expression 10><priority 11>w(g<expression 11>)b")
				("expression 10","<expression 09><priority 10>w(g<expression 09>)b")
				("expression 09","<expression 08><priority 09>w(g<expression 08>)b")
				("expression 08","<expression 07><priority 08>w(g<expression 07>)b")
				("expression 07","<expression 06><priority 07>w(g<expression 06>)b")
				("expression 06","<expression 05><priority 06>w(g<expression 05>)b")
				("expression 05","<expression 04><priority 05>w(g<expression 04>)b")
				("expression 04","<expression 03><priority 04>w(g<expression 03>)b")
				("expression 03","<expression 02><priority 03>wg<expression 03>")
				("expression 02","<priority 02>w(g)b<expression 01>")
				("expression 01","<priority 01>w(g)b<expression 00>")
				("expression 00","<([expression 15])>i(Nqign)i(Sqign)i(Iqign)i(Fqign)w'expected primery't")

				("([expression 15])", "'('qnirg<expression 15>')'qnw'expected )'t")
			// priority
				("priority 01","cf'@'o'$'o'*'o'&'o'++'o'--'o")
				("priority 02","cf'!'o'~'o'+'o'-'o")
				("priority 03","cf'**'o")
				("priority 04","cf'*'o'%'o'/'o")
				("priority 05","cf'+'o'-'o")
				("priority 06","cf'>>'o'<<'o")
				("priority 07","cf'^'o")
				("priority 08","cf'&'o")
				("priority 09","cf'|'o")
				("priority 10","cf'<='o'<'o'>'o'>='o")
				("priority 11","cf'=='o'!='o")
				("priority 12","cf'&&'o")
				("priority 13","cf'||'o")
				("priority 14","cf'=*'o'=-'o'=+'o'=%'o'=/'o'=<<'o'=>>'o'=&'o'=|'o'=^'o'=>'o")
				("priority 15","cf','o")
			// DEBUGing interrupts
			("DBG","%dbg \n()%dbg out()") // overload empty dbg
			("DBG","%dbg \n('\n'[0]qi('\n'v))%dbg out(j' 'v)") // overload dbg
			// Elementary interrupts
			("skip \n","'\n'[0]qwgb") // skip all simbol '\n'(eoln)
			("skip \n;","('\n'[0]q';'o)wgb") // skip all simbol '\n'(eoln) and ';'
			("eof","Gq") // push true if end of file here into @param flag
			("neof","Gqn") // push true if not end of file here into @param flag
			// End of declaratio vector interrupts
			; //getc(stdin);
			auto t0 = time(0); cntxt
			("Start"); // run context control
			auto t1 = time(0); printf("[dt: %lld(%lld-%lld)]",t1-t0,t1,t0);
			// End
	} catch(string err_msg)
	{
		cout << "Critical error: " << err_msg << "\n";
	}

	if(cntxt.in())fclose(cntxt.in());

	return 0;
}
#endif

//		("dbg \n","")
//		("dbg out","")
//		("dbg \n","'\n'[0]qi('\n'v)")
//		("dbg out","j' 'v")

//		("Start","p<bof>vg<check end><\1\62>(gGqn)w(jTqnw' 'v)b<eof>v<b><air>v<last>")
//		("check end","Gqw'unexpected end of file't")
//		("last","'\nI throw, because it is end of file\n'v<check end>")
//		("eof","'+---------------+\n|  End of File  |\n+---------------+\n'")
//		("bof","'+---------------+\n| Begin of File |\n+---------------+\n'")
//		("air","'All Right\n'")
//		("b","'I go b and call'v<c><then>")
//		("c","' lex c\n'v")
//		("then","'then go here and say: 'v");
