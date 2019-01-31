#if __cplusplus < 201103L
# include <bits/c++0x_warning.h>
#else
#include "context.hpp"
#include <time.h>

using namespace std;

int main()
{
	context::context cntxt;
	cntxt.in() = fopen("test.da","rb");
	cntxt.out() = stdout;

	try{
		cntxt
			// Replace standart interrupt
//			("\1",[&](){})
			("start",[&](){printf("[start work]\n");})
			("end",[&](){printf("\n[end work]");})
			("",[&](){ cntxt.control_obj.})
			// End of declaratio interrupts
			// Start interrupt
			("Start","pg{start}<program>{end}u")
			// Basic interrupts
			("program","<neof>w(<dbg \n><skip \n><dbg out>g)b")
			("operator","<if>i<while>i<for>i<switch>i<{[operators]}>i<expression>w'uncorrect operator't")
			("if","'if'qnirg<skip \n>'('qnw'expected \"(\"'tg<expression>'('qnw'expected \")\"'tg<skip \n><operator><skip \n>'else'qnirg<operator><skip \n>cf")
			("while","")
			("for","")
			("switch","")
			("{[operators]}","'{'qnirg<skip \n>(<eof>'}'on)w(<operator><skip \n;>)b<eof>w'finde eof, expected \"}\"'t'}'qwgrcf")
			("expression",
				"<goto[label]>i<:[lable]>i<::=[operator]>i<expression 15>w'uncorrect expression't")
			("goto[label]","'goto'qnirgNqnw'expected lable identity't")
			(":[lable]","':'qnirgNqnw'expected defenition lable identity't")
			("::=[operator]","'::='qnirg<operator>cf")
			// expression
				("expression 15","")
				("expression 14","")
				("expression 13","")
				("expression 12","")
				("expression 11","")
				("expression 10","")
				("expression 09","")
				("expression 08","")
				("expression 07","")
				("expression 06","")
				("expression 05","")
				("expression 04","")
				("expression 03","")
				("expression 02","")
				("expression 01","")
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
