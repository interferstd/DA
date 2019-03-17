#if __cplusplus < 201103L
#error "must use '-std=c++11' or '-std=c++14' or '-std=c++17'"
#else
#include "context.hpp"
#include "tid_table.hpp"
#include "errors.hpp"
#include <time.h>

using namespace std;

int main()
{
	context::context cntxt;

	dllist::dlist<string> stack_types, stack_operations; // interpret type count

	string name = "", type = ""; // additional
	tids::tid_table<string, string> var_table; // var
	tids::tid_elem<string, char> lable_table; tids::tid_elem<string, localization> need_lable; // lable
	tids::function_desribe<string, string> function_to_init; // param
	tids::tid_elem<string, tids::function_desribe<string, string>> function_table; tids::tid_elem<string, localization> need_function; // func

	AVLHashMap::AVLHML<string,char> types; // types

	types["int"],types["char"],types["bool"],types["float"];
	string buf1 = "", buf2 = "", buf3 = "";

	try{
		cntxt
			// init
			.set_in("woyna_i_mir.da")
			.set_out(stdout)
			// Replace standart interrupt
			("push space",[&](){ var_table.push_space(); })
			("pop space",[&](){ var_table.pop_space(); })
			("push var id",[&](){ var_table.push_id(name, type); })
			("available var",[&](){ cntxt.control_obj.flag() = var_table.available(name); })
			("declarable var",[&](){ cntxt.control_obj.flag() = var_table.declarable(name); })
			("colect type",[&](){ type += cntxt.control_obj.last().content; })
			("clear type",[&](){ type = ""; })
			("show type",[&](){ fprintf(cntxt.control_obj.lexical_obj.out(), "%s", type.c_str()); })
			("push name",[&](){ name = cntxt.control_obj.last().content; })
			("declarable lable",[&](){ cntxt.control_obj.flag() = !lable_table.at(name); })
			("define lable",[&](){ lable_table[name] = 'L'; })
			("need lable",[&](){ need_lable[name] = cntxt.control_obj.lexical_obj.local; })
			("check lables",[&](){ need_lable.map([&](pair<string, localization> p)
				{ if(!lable_table.at(p.first)){ name = p.first; cntxt.control_obj.lexical_obj.local = p.second; throw semantic_msg("lable not defined: "); } }); })
			("function format",[&](){ cntxt.control_obj.message() = function_to_init.format(); })
			("set return type",[&](){ function_to_init.return_type() = type; })
			("add type",[&](){ function_to_init.types.add(type); })
			("add name",[&](){ function_to_init.ids.add(name); })
			("clear function",[&](){ function_to_init = tids::function_desribe<string, string>(); })
			("push function id",[&](){ function_to_init.id() = cntxt.control_obj.last().content; })
			("need function",[&](){ need_function[function_to_init.format()] = cntxt.control_obj.lexical_obj.local; })
			("init function",[&](){ function_to_init.init(var_table.current()); function_table[function_to_init.format()] = function_to_init; })
			("check function",[&](){ need_function.map([&](pair<string, localization> p)
				{ if(!function_table.at(p.first)){ name = p.first; cntxt.control_obj.lexical_obj.local = p.second; throw semantic_msg("function not defined: "); } }); })
			("\1g",[&](){ cntxt.control_obj.last() = cntxt.control_obj.lexical_obj.get(); if(cntxt.control_obj.last().content[0] == '\n') buf1 = buf2, buf2 = buf3+'\n', buf3 = ""; else (buf3 += ' ') += cntxt.control_obj.last().content; })
			("throw semantic",[&](){ throw semantic_msg(cntxt.control_obj.message()); })
			("is type",[&](){ cntxt.control_obj.flag() = types.search(cntxt.control_obj.last().content); })
			// End of declaratio interrupts
			// Start interrupt
			("Start","pg{push space}<program>{pop space}u")
			// Basic interrupts
			("program","<neof>w(<dbg \n><skip \n><dbg out><declarations>)b{check lables}")
			("operator","<;>i<declaration var>i<if>i<while>i<for>i<{[operators]}>i<isn`t \n or ;>i<expression>w'uncorrect operator't")
			("if","'if'qnirg<skip \n>'('qnw'expected \"(\"'tg<skip \n><expression><skip \n>')'qnw'expected \")\"'tg<skip \n><operator><skip \n>'else'qnwnrg<operator><skip \n>cf")
			("while","'while'qnirg<skip \n>'('qnw'expected \"(\"'tg<expression><skip \n>')'qnw'expected \")\"'tg<skip \n><operator><skip \n>'else'qnwnrg<operator><skip \n>cf")
			("for","'for'qnir{push space}g<skip \n>'('qnw'expected \"(\"'tg<skip \n><body for><skip \n>')'qnw'expected \")\"'tg<skip \n><operator><skip \n>'else'qnwnrg<operator><skip \n>cf")
			("body for","<skip \n><operator><is \n or ;>nw'expected \";\" or \"\\n\"'tg<skip \n><operator><is \n or ;>nw'expected \";\" or \"\\n\"'tg<skip \n><operator><skip \n>{pop space}")
			("{[operators]}","'{'qnir{push space}g<skip \n>(<eof>'}'on)w(<operator><skip \n;>)b'}'qnw'expected \"}\"'t{pop space}gcf")
			("([expression 15]) or ([type])[expression 14]", "'('qnirgn({is type}nirg('*'q'**'o)w(g)bcf)e('{push operation}'')'qnw'expected \")\"'tg<expression 14>cf)i(<expression 15>')'qnw'expected \")\"'tg)")
			("expression",
				"<;>i<goto[label]>i<:[lable]>i<::=[operator]>i<expression 15>w'uncorrect expression't")
			("goto[label]","'goto'qnirgNqnw'expected lable identity't{push name}{need lable}g")
			(":[lable]","':'qnirgNqnw'expected defenition lable identity't{push name}{declarable lable}nw'lable is define: 't{define lable}g")
			("::=[operator]","'::='qnirg<operator>cf")
			("declarations","<declaration var>i<declaration func>w'uncorrect declaration'")
			("declaration var","{is type}nir{colect type}g('*'q'**'o)w({colect type}g)b<name>{push name}{declarable var}nw'id is defined: '{throw semantic}{push var id}g%init param('('qi(g<expression 15>')'qnw'expected \")\"'tg))(','q)w(g<name>{push name}{push var id}g<init param>)b<is \n or ;>nw'ecxpected \"\\n\" or \";\"'tg{clear type}")
			("declaration func","{clear function}{push space}<name>{push function id}g<skip \n>'::='qnw'expected ::='tg<skip \n><description types><skip \n><description names>('{'v{function format}v'}\n'v){init function}<skip \n><description branches><skip \n>{pop space}")
			("description types", "(<type>ir{add type}(','q)w(g<skip \n><type>{add type})b)<skip \n>('->'qnirg<type>{set return type})<skip \n>")
			("description names", "'('qnir(g<skip \n>Nqnir{push name}{add name}g(','q)w(g<skip \n><name>{push name}{add name}g)b)<skip \n>')'qnw'expected \")\"'tg<skip \n>")
			("description branches", "(<operator><is \n or ;>wgr'excepted ; or \\n't)('('q)w(g<expression 14>(','q)w(g<expression 14>)b')'qnw'expected \")\"'tg<operator><is \n or ;>nw'ecxpected \"\\n\" or \";\"'tg)b")
			("name","Nqnw'excepted identity't")
			("type", "{is type}nir{clear type}{colect type}g('*'q'**'o)w({colect type}g)bcf")

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
				("expression 01","<priority 01>w(g)b<expression 00>('++'q'--'o)ig")
				("expression 00","<([expression 15]) or ([type])[expression 14]>i(Nq{push name}wg('('qe({available var}nw'not defined var: '{throw semantic})i(g(<skip \n>')'qn)w(<expression 14>','q')'onw'expected \",\"'t','qig)bgcf)n)n)i(Sqign)i(Iqign)i(Fqign)w'expected primery't")

			// priority
				("priority 01","'@'q'$'o'*'o'&'o'++'o'--'o")
				("priority 02","'!'q'~'o'+'o'-'o")
				("priority 03","'**'q")
				("priority 04","'*'q'%'o'/'o")
				("priority 05","'+'q'-'o")
				("priority 06","'>>'q'<<'o")
				("priority 07","'^'q")
				("priority 08","'&'q")
				("priority 09","'|'q")
				("priority 10","'<='q'<'o'>'o'>='o")
				("priority 11","'=='q'!='o")
				("priority 12","'&&'q")
				("priority 13","'||'q")
				("priority 14","'=*'q'=-'o'=+'o'=%'o'=/'o'=<<'o'=>>'o'=&'o'=|'o'=^'o'=>'o")
				("priority 15","','q")
			// DEBUGing interrupts
			("DBG","%dbg \n()%dbg out()") // overload empty dbg
//			("DBG","%dbg \n('\n'[0]qi('\n'v))%dbg out(j' 'v)") // overload dbg
			// Elementary interrupts
			(";","';'qign") // set flag if is '\n' or ';'
			("is \n or ;","'\n'[0]q';'o") // set flag if is '\n' or ';'
			("isn`t \n or ;","'\n'[0]q';'on") // set flag if is not ('\n' or ';')
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
	}
	catch(preprocess_msg msg)
	{
		fprintf(cntxt.out(), "================================================\nFile: %s\nLine: %llu\n",
			cntxt.control_obj.lexical_obj.local.first.c_str(),
			cntxt.control_obj.lexical_obj.local.second-((cntxt.control_obj.last().content[0]=='\n')?cntxt.control_obj.last().content.length():0));
		cout << "Critical preprocessing error:\n " << msg.content << "\n================================================\n";
	}
	catch(context_msg msg)
	{
		fprintf(cntxt.out(), "================================================\nFile: %s\nLine: %llu\n",
			cntxt.control_obj.lexical_obj.local.first.c_str(),
			cntxt.control_obj.lexical_obj.local.second-((cntxt.control_obj.last().content[0]=='\n')?cntxt.control_obj.last().content.length():0));
		cout << "Critical context error:\n " << msg.content << "\n================================================\n";
		cout << buf1+buf2+buf3+"\n";
		stdout << cntxt.control_obj.last();
	}
	catch(semantic_msg msg)
	{
		fprintf(cntxt.out(), "================================================\nFile: %s\nLine: %llu\n",
			cntxt.control_obj.lexical_obj.local.first.c_str(),
			cntxt.control_obj.lexical_obj.local.second-((cntxt.control_obj.last().content[0]=='\n')?cntxt.control_obj.last().content.length():0));
		cout << "Critical semantic error:\n " << msg.content << name
//			((err_msg == "not defined var: " || err_msg == "id is defined: " ||
//			 err_msg == "lable is define: " || err_msg == "lable not defined: ")?name:"")
			<< "\n================================================\n";
		if(!(msg.content == "lable not defined: ")){
			cout << buf1+buf2+buf3+"\n";
			stdout << cntxt.control_obj.last();
		}
	}
	catch(string err_msg)
	{
		fprintf(cntxt.out(), "================================================\nFile: %s\nLine: %llu\n",
			cntxt.control_obj.lexical_obj.local.first.c_str(),
			cntxt.control_obj.lexical_obj.local.second-((cntxt.control_obj.last().content[0]=='\n')?cntxt.control_obj.last().content.length():0));
		cout << "Critical error: " << err_msg <<
			((err_msg == "not defined var: " || err_msg == "id is defined: " ||
			 err_msg == "lable is define: " || err_msg == "lable not defined: ")?name:"")
			<< "\n================================================\n";
		if(!(err_msg == "lable not defined: ")){
			cout << buf1+buf2+buf3+"\n";
			stdout << cntxt.control_obj.last();
		}
	}

	return 0;
}
#endif
