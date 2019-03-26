#if __cplusplus < 201103L
#error "must use '-std=c++11' or '-std=c++14' or '-std=c++17'"
#else

//#define DEBUG
#include "context.hpp"
#include "tid_table.hpp"
#include "errors.hpp"
#include <time.h>
#include <algorithm>

using namespace std;

void* main_analysis(void * arg)
{
	DBG("[START MAIN]");
	dllist::dlist<string> stack_types, stack_operations; string buf; // interpret type count

	string name = "", type = ""; // additional
	tids::tid_table<string, string> var_table; // var
	tids::tid_elem<string, char> lable_table; tids::tid_elem<string, localization> need_lable; // lable
	tids::function_desribe<string, string> function_to_init; tids::tid_elem<string, tids::function_desribe<string, string>> function_table; tids::tid_elem<string, localization> need_function; // func
	// param

	AVLHashMap::AVLHML<string,char> types; // types

	DBG("[same init]")
	types["int"],types["char"],types["float"];
	string buf1 = "", buf2 = "", buf3 = "";
	DBG("[same inited]")

 function<void(string, bool)> interpret = [&](string op, bool unary)
	{
		DBG("[interpret]");
		function<bool(string&)> is_ptr = [](string &s)->bool{ for(auto &e: s) if(e=='*') return true; return false; };
		function<bool(string&)> is_ref = [](string &s)->bool{ for(auto &e: s) if(e=='&') return true; return false; };
		auto clear_type = [](string s)->string{ string ans = ""; if(s.length()<1) return ""; ans.reserve(s.length()); for(string::iterator it = s.begin(); it < s.end(); ++it) if(*it!='&'&&*it!='*') ans.push_back(*it); return ans; };
		auto unref_type = [](string s)->string{ string ans = ""; if(s.length()<1) return ""; ans.reserve(s.length()); for(string::iterator it = s.begin(); it < s.end(); ++it) if(*it!='&') ans.push_back(*it); return ans; };
		auto cast = [](string s)->string { string ans; if(s.length()<3) return ""; ans.reserve(s.length()-2); for(string::iterator it = s.begin()+1; it < s.end()-1; ++it) ans.push_back(*it); return ans; };
		auto unnamed = [&is_ref,&name](string s)->string { if(is_ref(s)) for(auto &e: s) { if(e=='&') return (e='*',s); } else throw semantic_msg(string("Can`t unnamed ")+(name="",s)); };
		auto named = [&is_ptr,&name](string s)->string { if(is_ptr(s)) { reverse(s.begin(),s.end()); for(auto &e: s) { if(e=='*') { e='&'; reverse(s.begin(),s.end()); return s; } } } else throw semantic_msg(string("Can`t named ")+(name="",s)); };
		map<string, int> inst =
			{{"*",3},{"/",1},{"+",3},{"-",3},{"==",2},{"!=",2},{"<=",2},{">=",2},{"<",2},{">",2},{"&&",2},{"||",2},{"@",3},{"$",3},{"&",3}};

		switch (inst[op]) {
			case 0:
			{
				if(op[0] == '(')
				{
					op = cast(op);
					string op1 = *stack_types.back();
					stack_types.tb();
					if(is_ptr(op) == is_ptr(op1) || (is_ptr(op) && unref_type(op1)=="int"))
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
					stack_types.pb(unref_type(op1));
				}else
				{
					name = "";
					throw semantic_msg(string("Can`t done binary operation ")+op2+" "+op+" "+op1+(name=""));
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
					throw semantic_msg(string("Can`t done logic operation ")+op2+" "+op+" "+op1+(name=""));
				}
			} break;
			case 3:
			{
				if(!unary) goto UNUNARY;
				string op1 = *stack_types.back();
				stack_types.tb();
				if(op == "@") {	if(unref_type(op1)=="int") stack_types.pb("*"); else throw semantic_msg(string("Can`t done operation ")+op+op1+"\n expected @int"+(name="")); }
				else if(op == "$") { if(is_ptr(op1)) stack_types.pb(""); else throw semantic_msg(string("Can`t done operation ")+op+op1+"\n expected $[type]*"+(name="")); }
				else if(op == "*") { stack_types.pb(named(op1)); }
				else if(op == "&") { stack_types.pb(unnamed(op1)); }
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
		if(*stack_operations.back() == "") interpret_do(); else
		{
			while(*stack_operations.back() != "" && stack_operations.len > 0)
			{
				interpret(*stack_operations.back(), true);
				stack_operations.tb();
			}
			if(*stack_operations.back() == "" && stack_operations.len > 0) stack_operations.tb();;
		}
		DBG("[do while end]");
	};

	DBG("[START WORK]");
	context::context cntxt;
	DBG("[INIT CONTEXT]");

	try{
		cntxt
			// Replace standart interrupt
			("push buf in content",[&](){ cntxt.control_obj.last().content = buf; })
			("push space",[&](){ var_table.push_space(); })
			("pop space",[&](){ var_table.pop_space(); })
			("push var id",[&](){ DBG("[PID:%s]",type.c_str()); var_table.push_id(name, type); })
			("available var",[&](){ cntxt.control_obj.flag() = var_table.available(name); })
			("var type in buf",[&](){ buf += var_table[name]; })
			("declarable var",[&](){ cntxt.control_obj.flag() = var_table.declarable(name); })
			("colect type",[&](){ type += cntxt.control_obj.last().content; })
			("clear type",[&](){ type = ""; })
			("check init",[&](){ if(type != *stack_types.back()) throw semantic_msg(string("Uncorect initialization ")+type+" of "+*stack_types.back()+(name="")); })
			("show type",[&](){ global_rw::wdlock(); fprintf(cntxt.control_obj.lexical_obj.out(), "{%s}", type.c_str()); global_rw::wulock(); })
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
			("show buf",[&](){ global_rw::wdlock(); fprintf(cntxt.control_obj.lexical_obj.out(), "{%s}", buf.c_str()); global_rw::wulock(); })
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
			.set_out(stdout)
			.set_in("woyna_i_mir.da")
			// End init
			; //getc(stdin);
			auto t0 = time(0); cntxt
			("Start"); // run context control
			auto t1 = time(0); printf("[dt: %lld(%lld-%lld)%d:%d]",t1-t0,t1,t0,stack_types.len,stack_operations.len);
			// End
	}
	catch(preprocess_msg msg)
	{
		global_rw::wdlock();
		fprintf(cntxt.out(), "\n================================================\nFile: %s\nLine: %llu\n",
			cntxt.control_obj.lexical_obj.local.first.c_str(),
			cntxt.control_obj.lexical_obj.local.second-((cntxt.control_obj.last().content[0]=='\n')?cntxt.control_obj.last().content.length():0));
		cout << "Critical preprocessing error:\n " << msg.content << "\n================================================\n";
		global_rw::wulock();
	}
	catch(context_msg msg)
	{
		global_rw::wdlock();
		fprintf(cntxt.out(), "\n================================================\nFile: %s\nLine: %llu\n",
			cntxt.control_obj.lexical_obj.local.first.c_str(),
			cntxt.control_obj.lexical_obj.local.second-((cntxt.control_obj.last().content[0]=='\n')?cntxt.control_obj.last().content.length():0));
		cout << "Critical context error:\n " << msg.content << "\n================================================\n";
		cout << buf1+buf2+buf3+"\n";
		stdout << cntxt.control_obj.last();
		global_rw::wulock();
	}
	catch(semantic_msg msg)
	{
		global_rw::wdlock();
		fprintf(cntxt.out(), "\n================================================\nFile: %s\nLine: %llu\n",
			cntxt.control_obj.lexical_obj.local.first.c_str(),
			cntxt.control_obj.lexical_obj.local.second-((cntxt.control_obj.last().content[0]=='\n')?cntxt.control_obj.last().content.length():0));
		cout << "Critical semantic error:\n " << msg.content << name
			<< "\n================================================\n";
		if(!(msg.content == "lable not defined: ")){
			cout << buf1+buf2+buf3+"\n";
			stdout << cntxt.control_obj.last();
		}
		global_rw::wulock();
	}
	catch(string err_msg)
	{
		global_rw::wdlock();
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
		global_rw::wulock();
	}

	pthread_exit(0);
}

int main(int argc, char** argv)
{
	pthread_t thread; void * res;
	pthread_create(&thread, NULL, main_analysis, NULL); // make istream output (ret(param))
	pthread_join(thread, &res);

	return 0;
}


#endif
