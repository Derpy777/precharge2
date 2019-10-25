#include "shared_mmap.hpp"
#include <system_error> // for std::error_code
#include <cstdio> // for std::printf
#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <experimental/filesystem>
#include <sys/stat.h>
//#include <windows.h>
#include <fstream>




using namespace std;
namespace filesys = std::experimental::filesystem;



class Credits
{
	
	std::string _poney = "+++:++++++:++:++:+++::+::+::+::+::+::+::+::+::+::+::+::+\n\
::::::+::+:++:++:+++::+::+::+::+::+::+::+::+::+::+::+::+\n\
::::::::::::::++++:+::++++::+::+::+::+::+::+::+::+::+::+\n\
++++:++:++:++:++#@@@@@@#=*=##*++::+::+:++::+::+::+::+::+\n\
:+:::::::+:++:++++=@@@@@@@@@@###+-.--..-.....-----:::::+\n\
++++++++++:+=##################*.....................-++\n\
::::::::+=####################+.....................::::\n\
:+::::+*####@@#@@@@@@#########*...................-++::+\n\
++++++=##*#@@@@@@@@######+#####:................:+++++++\n\
::::+==++@@@@@@@@######*.*#####-.............-::::::::::\n\
:+:+++:+@@@@@@@#######*..-*###*...........:*::++::::::::\n\
+++++++@=:::++*=######....-###-......:=######+++++::::::\n\
::::::#@@#:----+#####+....:###@=-...-#########+:::::::::\n\
:+:::*@@@@@#+-*######-.-#@*#+#@:....+##########+:+::+::+\n\
+++++#@@@@#########=-:W@*.=:.-W-...-########@###*:++++++\n\
::::*@=@@########=--#@@@#:...-W-..-####@####@@#*#=::::::\n\
:+::*=*@########-..#@@@@@:...#@:.-####@#####@@#=::+++::+\n\
+++++:*@#####=++-..*@@@@=.:@#-..-###@@@####@@@@@++++++++\n\
::::::=####=+::::...+:-+#+-....-##=@@@@####@@@@@=+::::::\n\
:+:::+####+::::::...--.........*#-=@@@####@@@@@@@+::+::+\n\
++++++*#=+++++:-..:...........-=-:@@@####@@@@@@@@*++++++\n\
::::::+=:::::::-...............-.=@@@###@@@@@@#@@=::::::\n\
:+::::+::+:++:++::--.........-..-@@@####@@@@@@=+@*::+::+\n\
+++++++++++++++++++::-..--*##...+@@@####@@@@@@*+++++++++\n\
:::::::::::::::::::::::::=##+...=@@@###@@@@@@#::::::::::\n\
:+::::+::+:++:++:+++::++###=....@@#####@@@@@@*+::+::+::+\n\
++++++++++++++++++++++*####-....@@*#####@@@@=+++++++++++\n\
::::::::::::::::::::+####@:.....*@+#####@@@@+:::::::::::\n\
:+::::+::+:++:++:+*#=*#@@:.......+*+####@@@=::+::+::+::+\n\
+++++++++++++++++++++@@@@:.........-####@@@+++++++++++++";


	std::string _secret = "Petite dédicasse à la GDC, vive la Master-Race XD\n";


	std::string _bonjour = "-------------------------------------------\n\
Programme réalisé et codé en C++ par Derpy\n\
-------------------------------------------";

	std::string _inRam = "Les données ont été correctement mises dans la RAM, vous pouvez lancer le jeu...\n\
Quand vous aurez fini de jouer, pressez une touche, les données seront alors retirées de la RAM...";


	std::string _fini = "Les données ont été correctement enlevées de la RAM...";

	std::string _usage = "\nUSAGE\n";

	std::string _version = "1.0.0";


public:

	std::string getBonjour()
	{
		return _bonjour;
	}

	std::string getInRam()
	{
		return _inRam;
	}

	std::string getFini()
	{
		return _fini;
	}

	std::string getUsage()
	{
		return _usage;
	}

	std::string getVersion()
	{
		return _version;
	}

	std::string getPoney()
	{
		return _poney;
	}

	std::string getSecret()
	{
		return _secret;
	}

};



bool IsPathExist(const std::string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}



/*
* Get the list of all files in given directory and its sub directories.
*
* Arguments
* 	dirPath : Path of directory to be traversed
* 	dirSkipList : List of folder names to be skipped
*
* Returns:
* 	vector containing paths of all the files in given directory and its sub directories
*
*/
std::vector<std::string> getAllFilesInDir(const std::string &dirPath, 	const std::vector<std::string> dirSkipList = { })
{
// Create a vector of string
std::vector<std::string> listOfFiles;
try {
// Check if given path exists and points to a directory
if (filesys::exists(dirPath) && filesys::is_directory(dirPath))
{
// Create a Recursive Directory Iterator object and points to the starting of directory
filesys::recursive_directory_iterator iter(dirPath);
// Create a Recursive Directory Iterator object pointing to end.
filesys::recursive_directory_iterator end;
// Iterate till end
while (iter != end)
{
// Check if current entry is a directory and if exists in skip list
if (filesys::is_directory(iter->path()) &&
(std::find(dirSkipList.begin(), dirSkipList.end(), iter->path().filename()) != dirSkipList.end()))
{
// Skip the iteration of current directory pointed by iterator
#ifdef USING_BOOST
// Boost Fileystsem  API to skip current directory iteration
iter.no_push();
#else
// c++17 Filesystem API to skip current directory iteration
iter.disable_recursion_pending();
#endif
}
else
{
// Add the name in vector
if (not filesys::is_directory(iter->path()))
{
	listOfFiles.push_back(iter->path().string());
}
}
error_code ec;
// Increment the iterator to point to next entry in recursive iteration
iter.increment(ec);
if (ec) {
std::cerr << "Error While Accessing : " << iter->path().string() << " :: " << ec.message() << '\n';
}
}
}
}
catch (std::system_error & e)
{
std::cerr << "Exception :: " << e.what();
}
return listOfFiles;
}



/*
unsigned long long getFreeSystemMemory()
{
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullAvailPhys/(1024*1024);
}*/



std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}



constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}



void travail(std::string racine)
{
	std::vector<std::string> listeDeFichiers;
  std::vector<mio::shared_mmap_sink*> listeDeMaps;
  unsigned long memoireLibre = 0;
  unsigned long nbMbLus = 0;


  //memoireLibre = getFreeSystemMemory();


	std::cout << "Découverte de la liste des fichiers du jeu..." << '\n' ;
  listeDeFichiers = getAllFilesInDir(racine);


	std::cout << "Mappage en mémoire des fichiers du jeu..." << '\n' ;
  for(int i=0; i<listeDeFichiers.size(); ++i)
	{
		
    nbMbLus = nbMbLus + filesize(listeDeFichiers[i]);

    if(nbMbLus < memoireLibre)
    {
      listeDeMaps.push_back(new mio::shared_mmap_sink(listeDeFichiers[i]));
    }
    else
    {
      std::cout << "La RAM est pleine, la mise en mémoire a donc été arrêté..." << '\n';
      break;
    }
	}

  std::cout << "Le jeu a correctement été mis en mémoire..." << '\n' ;
  std::cout << "Vous pouvez lancer le jeu maintenant" << '\n' ;
  std::cout << "Lorsque vous aurez fini de jouer, appuyez sur une touche, le processus de libération de la mémoire commencera..." << '\n' ;
  
  //system("pause");
  system("read a");

  std::cout << "libération de la mémoire..." << '\n' ;

  for(int i=0; i<listeDeMaps.size(); ++i)
  {
    listeDeMaps[i]->unmap();
    delete listeDeMaps[i];
  }

  std::cout << "Le jeu a correctement été retiré de la mémoire..." << '\n' ;

}



int main(int argc, char** argv)
{

	Credits credits;


    std::cout << credits.getBonjour() << '\n';


    if((argc<2) or (argc>3))
    {
    	std::cout << "Nombre de paramètres incorrect !" << '\n';
    	std::cout << credits.getUsage() ;
    	return 1;
    }


    switch (str2int(argv[1]))
    {
  		
    	case str2int("-r"):
    		if(argc == 3)
    		{
  				if(IsPathExist(argv[2]))
  				{
  					travail(argv[2]);
  				}
  				else
  				{
  					std::cout << "Le répertoire n'existe pas !" << '\n';
  				}

  			}
  			else
  			{
  				std::cout << "Nombre de paramètres incorrect !" << '\n';
    			std::cout << credits.getUsage() ;
    			return 1;
  			}
    		break;
  		case str2int("-v"):
  			std::cout << "Version du programme : " << credits.getVersion() << '\n' ;
    		break;
  		case str2int("-?"):
  			std::cout << credits.getUsage();
    		break;
    	case str2int("--memoire"):
  			std::cout << "RAM libre : ????? Mb" << '\n';
    		break;
    	case str2int("--secret"):
  			std::cout << credits.getPoney() << "\n\n\n" << credits.getSecret() << '\n';
    		break;
    	default:
    		std::cout << credits.getUsage();
	}



    return 0;
}