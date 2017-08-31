#include "PTestTool.h"

namespace Grep 
{
	namespace PTestTool 
	{
		namespace {

			char transformChar(const char c,const bool ignoreCaseSensitivity) noexcept
			{
				if (!ignoreCaseSensitivity) return c;
				if (('a' <= c && c <= 'z') || ('a' <= c && c <= 'z')) {

					return (char)((int)c + (int)('A' - 'a'));
				}
				return c;
			}

			bool pFunctionTest(const std::string & str,const std::string & word,const bool ignoreCaseSensitivity)
			{
				std::vector<int> arr(str.length());
				for (size_t i = 1; i < str.length(); i++)
				{
					int k = arr[i - 1];
					while (k > 0 && transformChar(str[i], ignoreCaseSensitivity) != transformChar(str[k], ignoreCaseSensitivity)) 
					{
						k = arr[k - 1];
					}
					if (transformChar(str[i], ignoreCaseSensitivity) == transformChar(str[k], ignoreCaseSensitivity)) 
					{
						k++;
					}
					arr[i] = k;
				}
				for (size_t i = word.length() + 1; i < str.length(); i++)
				{
					if (arr[i] == word.length())
					{
						return true;
					}
				}
				return false;
			}
		}

		int Test(const std::experimental::filesystem::directory_entry&  entry,const std::string & word,const bool ignoreCaseSensitivity) noexcept
		{
			try 
			{
				std::ifstream ifs(entry.path().string());

				//TODO:: add buffer reading, because line can be large.
				std::string line;
				int lineNumber = 1;
				while (std::getline(ifs, line))
				{
					char connector = static_cast<char>(1);
					std::string toTest = word + connector + line;
					if (pFunctionTest(toTest, word, ignoreCaseSensitivity)) 
					{
						return lineNumber;
					}
					lineNumber++;
				}
				return -1;
			}
			catch (...)
			{
				return -1;
			}
		}
	}
}
