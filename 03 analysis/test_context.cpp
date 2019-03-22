#if __cplusplus < 201103L
#error "must use '-std=c++11' or '-std=c++14' or '-std=c++17'"
#else

#define DEBUG
#include "context.hpp"
#include "tid_table.hpp"
#include "errors.hpp"
#include <time.h>

using namespace std;

int main()
{
	dllist::dlist<string> stack_types, stack_operations; string buf; // interpret type count

	string name = "", type = ""; // additional
	tids::tid_table<string, string> var_table; // var
	tids::tid_elem<string, char> lable_table; tids::tid_elem<string, localization> need_lable; // lable
	tids::function_desribe<string, string> function_to_init; tids::tid_elem<string, tids::function_desribe<string, string>> function_table; tids::tid_elem<string, localization> need_function; // func
	// param

	AVLHashMap::AVLHML<string,char> types; // types

	types["int"],types["char"],types["float"];
	string buf1 = "", buf2 = "", buf3 = "";

	function<void(string, bool)> interpret = [&](string op, bool unary)
	{
		DBG("[interpret]");
		auto is_ptr = [](string s)->bool{ for(auto &e: s) if(e=='*') return true; return false; };
		auto is_ref = [](string s)->bool{ for(auto &e: s) if(e=='&') return true; return false; };
		auto clear_type = [](string s)->string{ string ans = ""; if(s.length()<1) return ""; ans.reserve(s.length()); for(string::iterator it = s.begin(); it < s.end(); ++it) if(*it!='&'&&*it!='*')ans.push_back(*it); return ans; };
		auto unref_type = [](string s)->string{ string ans = ""; if(s.length()<1) return ""; ans.reserve(s.length()); for(string::iterator it = s.begin(); it < s.end(); ++it) if(*it!='&') ans.push_back(*it); return ans; };
		auto cast = [](string s)->string { string ans; if(s.length()<3) return ""; ans.reserve(s.length()-2); for(string::iterator it = s.begin()+1; it < s.end()-1; ++it) ans.push_back(*it); return ans; };
		map<string, int> inst =
			{{"*",1},{"/",1},{"+",3},{"-",3},{"==",2},{"!=",2},{"<=",2},{">=",2},{"<",2},{">",2},{"&&",2},{"||",2},{"@",3}};

		switch (inst[op]) {
			case 0:
			{
				if(op[0] == '(')
				{
					op = cast(op);
					string op1 = *stack_types.back();
					stack_types.tb();
					if(is_ptr(op) == is_ptr(op1))
					{
						stack_types.pb(op);
					}else
					{
						name = "";
						throw semantic_msg(string("Can`t done operation (")+op+") "+op1);
					}
				}else
				{
					name = "";
					throw semantic_msg("A lot of problems...");
				}
			} break;
			case 1:
			{
				UNUNARY:
				string op1 = *stack_types.back();
				stack_types.tb();
				string op2 = *stack_types.back();
				stack_types.tb();
				if(unref_type(op1) == unref_type(op2) && !is_ptr(op1) && !is_ptr(op2))
				{
					stack_types.pb(op1);
				}else
				{
					name = "";
					throw semantic_msg(string("Can`t done binary operation ")+op2+" "+op+" "+op1);
				}
			} break;
			case 2:
			{
				string op1 = *stack_types.back();
				stack_types.tb();
				string op2 = *stack_types.back();
				stack_types.tb();
				if(unref_type(op1) == unref_type(op2))
				{
					stack_types.pb("int");
				}else
				{
					name = "";
					throw semantic_msg(string("Can`t done logic operation ")+op2+" "+op+" "+op1);
				}
			} break;
			case 3:
			{
				if(!unary) goto UNUNARY;
				string op1 = *stack_types.back();
				stack_types.tb();
				if(op == "@") {	if(unref_type(op1)=="int") stack_types.pb("*"); else throw semantic_msg(string("Can`t done operation ")+op+op1+"\n expected @int"+(name="")); }
				else if(op == "$") { if(is_ptr(op1)) stack_types.pb(""); else throw semantic_msg(string("Can`t done operation ")+op+op1+"\n expected $[type]*"+(name="")); }
				else
				{
					
				}
			} break;
		};
	};
	function<void()> interpret_do = [&]()
	{
		DBG("[do]");
		if(stack_operations.len < 1) return;
		string op = *stack_operations.back();
		stack_operations.tb();
		if(op == "") return;
		interpret(op, false);
		DBG("[do end]");
	};
	function<void()> interpret_do_while = [&]()
	{
		DBG("[do while]");
		if(stack_operations.len < 1) return;
		dllist::dlist<string>::iterator op = stack_operations.back();
		if(*op == "") interpret_do(); else
		{
			while(*op != "") --op;
			++op;
			while(op != stack_operations.end()) interpret(*op, true), ++op;
			while(stack_operations.len > 0) if((*stack_operations.back()) != "")
				stack_operations.tb();
			else
			{
				stack_operations.tb();
				DBG("[do while end 1]");
				return;
			}
		}
		DBG("[do while end 2]");
	};

	context::context cntxt;
	
	try{
		cntxt
			// Replace standart interrupt
			("push space",[&](){ var_table.push_space(); })
			("pop space",[&](){ var_table.pop_space(); })
			("push var id",[&](){ DBG("[PID:%s]",type.c_str()); var_table.push_id(name, type); })
			("available var",[&](){ cntxt.control_obj.flag() = var_table.available(name); })
			("var type in buf",[&](){ buf += var_table[name]; })
			("declarable var",[&](){ cntxt.control_obj.flag() = var_table.declarable(name); })
			("colect type",[&](){ type += cntxt.control_obj.last().content; })
			("clear type",[&](){ type = ""; })
			("check init",[&](){ if(type != *stack_types.back()) throw semantic_msg(string("Uncorect initialization ")+type+" of "+*stack_types.back()+(name="")); })
			("show type",[&](){ fprintf(cntxt.control_obj.lexical_obj.out(), "{%s}", type.c_str()); })
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
			("clear type stack",[&](){ stack_types.clear(); })
			("back type to buf",[&](){ buf = *stack_types.back(); })
			("colect buf",[&](){ buf += cntxt.control_obj.last().content; })
			("mcolect buf",[&](){ buf += cntxt.control_obj.message(); })
			("clear buf",[&](){ buf = ""; })
			("show buf",[&](){ fprintf(cntxt.control_obj.lexical_obj.out(), "{%s}", buf.c_str()); })
			("is type",[&](){ cntxt.control_obj.flag() = types.search(cntxt.control_obj.last().content); })
			("do while",[&](){ DBG("[call do while]"); interpret_do_while(); DBG("[out call do while]"); })
			("do",[&](){ DBG("[call do]"); interpret_do(); DBG("[out call do]"); })
			("pause",[](){ system("pause"); })
			("\1\n",[](){})
			("\1 ",[](){})
			// End of declaratio interrupts
			// Loads
			.load(".cntxt")
			// End loads
			// init
			.set_in("woyna_i_mir.da")
			.set_out(stdout)
			// End init
			; //getc(stdin);
			auto t0 = time(0); cntxt
			("Start"); // run context control
			auto t1 = time(0); printf("[dt: %lld(%lld-%lld)%d:%d]",t1-t0,t1,t0,stack_types.len,stack_operations.len);
			// End
	}
	catch(preprocess_msg msg)
	{
		fprintf(cntxt.out(), "\n================================================\nFile: %s\nLine: %llu\n",
			cntxt.control_obj.lexical_obj.local.first.c_str(),
			cntxt.control_obj.lexical_obj.local.second-((cntxt.control_obj.last().content[0]=='\n')?cntxt.control_obj.last().content.length():0));
		cout << "Critical preprocessing error:\n " << msg.content << "\n================================================\n";
	}
	catch(context_msg msg)
	{
		fprintf(cntxt.out(), "\n================================================\nFile: %s\nLine: %llu\n",
			cntxt.control_obj.lexical_obj.local.first.c_str(),
			cntxt.control_obj.lexical_obj.local.second-((cntxt.control_obj.last().content[0]=='\n')?cntxt.control_obj.last().content.length():0));
		cout << "Critical context error:\n " << msg.content << "\n================================================\n";
		cout << buf1+buf2+buf3+"\n";
		stdout << cntxt.control_obj.last();
	}
	catch(semantic_msg msg)
	{
		fprintf(cntxt.out(), "\n================================================\nFile: %s\nLine: %llu\n",
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
		fprintf(cntxt.out(), "\n================================================\nFile: %s\nLine: %llu\n",
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
