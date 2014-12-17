#pragma once
#include <iostream>
#include <sstream>
//namespace TS {
// Probably an entity in itself, but we have it a bit specified here for Test output.
/* the whole point is that we want spaces to automatically get outputted for multi-level debug output
 so we have an unsigned int tablevel. since we are using spaces, numspaces = tablevel * 2or4. */
class TestStream : public std::ostream {
private:
	class TestStreamBuffer : public std::stringbuf {
	private:
		// or whatever you need for your application
		std::string m_marker;
		
	public:
		unsigned int tab_level;
		TestStreamBuffer(const std::string& marker) : m_marker(marker), tab_level (0) { }
		TestStreamBuffer() : tab_level(0) {}
		~TestStreamBuffer() {  pubsync(); }
		int sync()
		{
			string tabs("");
			for (int i=tab_level*2; i > 0; i--)
			{
				tabs.append(" ");
			}
			std::cout << tabs << str(); str("");  return !std::cout;
		}
	};
	
public:
	// Other constructors could specify filename, etc
	// just remember to pass whatever you need to TestStreamBuffer
	// int ChangeTabLevelTo(int n) { int cur = tablevel; tablevel = n; return cur; }
	//TestStream(const std::string& marker) { buf = new TestStreamBuffer(marker);
	//std::ostream(buf);}
	TestStream() : buf(), std::ostream(&buf) {}
	~TestStream() { /*delete rdbuf();*/ }
	
	unsigned int IncrementTabLevel()
	{
		unsigned int old = buf.tab_level;
		buf.tab_level++;
		return old;
	}
	unsigned int DecrementTabLevel()
	{
		unsigned int old = buf.tab_level;
		
		if (buf.tab_level == 0)
			buf.tab_level = 0;
		else buf.tab_level--;
		
		return old;
	}
	
	unsigned int ZeroTabLevel()
	{
		unsigned int old = buf.tab_level;
		
		buf.tab_level = 0;
		
		return old;
	}
	
	
	TestStreamBuffer buf;
};

extern TestStream test;
//}