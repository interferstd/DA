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

	dllist::dlist<string> stack_types, stack_operations; string buf; // interpret type count

	string name = "", type = ""; // additional
	tids::tid_table<string, string> var_table; // var
	tids::tid_elem<string, char> lable_table; tids::tid_elem<string, localization> need_lable; // lable
	tids::function_desribe<string, string> function_to_init; tids::tid_elem<string, tids::function_desribe<string, string>> function_table; tids::tid_elem<string, localization> need_function; // func
	// param

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
			("var type in buf",[&](){ buf = var_table[name]; })
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
			("push operation",[&](){ stack_operations.pb(buf); })
			("push type",[&](){ stack_types.pb(buf); })
			("colect buf",[&](){ buf += cntxt.control_obj.last().content; })
			("mcolect buf",[&](){ buf += cntxt.control_obj.message(); })
			("clear buf",[&](){ buf = ""; })
			("show buf",[&](){ fprintf(cntxt.control_obj.lexical_obj.out(), "%s", buf.c_str()); })
			("is type",[&](){ cntxt.control_obj.flag() = types.search(cntxt.control_obj.last().content); })
			("\1\n",[](){})
			("\1 ",[](){})
			// End of declaratio interrupts
			// Loads
			.load(".cntxt")
			// End loads
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
