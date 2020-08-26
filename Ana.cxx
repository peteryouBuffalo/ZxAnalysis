#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>

#include "src/ZxSelector.h"
#include "src/TTbarSelector.h"
#include "src/Global.h"

//=====================================================================================
// SetParameters Method 
//=====================================================================================

void SetParameters(std::string fName, glob::Parameters& para)
{
	// Go through the parameter file line by line.
	std::string line; std::ifstream myfile(fName);
	if(myfile.is_open())
	{
		while(getline(myfile,line))
		{
			// skip comment line start with "//"
			if(line.find("//") != std::string::npos) continue;

			// split the line into the proper bits
			std::vector<std::string> cont;
			std::vector<std::string> cont_no_space;
			std::string delim(" ");
			boost::split(cont, line, boost::is_any_of(delim));

			for (size_t i = 0; i < cont.size(); ++i)
			{
				if (cont[i].find_first_not_of(' ') != std::string::npos)
				{ cont_no_space.push_back(cont[i]); }
			}

			// If not of the proper format, let them know.
			if (cont_no_space.size() != 3) {
				std::cout << "\n Need name, value, value type. Will skip this";
				std::cout << " \"" << line << "\"" << std::endl;
			}
			// Otherwise, set the parameter
			else
			{
				std::string name = cont_no_space[0];
				if (cont_no_space[2] == "int") para.Set(name, std::stoi(cont[1]));
				if (cont_no_space[2] == "float") para.Set(name, std::stof(cont_no_space[1]));
			}

		}//end-while
		
		// close the file.
		myfile.close();
	}
	else std::cout << "Unable to open parameters file" << std::endl;
}



//=====================================================================================
// main Method 
//=====================================================================================
int main(int argc, char *argv[])
{
	std::cout << std::endl;
	std::cout << "=====================================================" << std::endl;
	std::cout << " Z+x Analysis call: " << std::endl;
	for (int i = 0; i < argc; ++i) std::cout << argv[i] << " ";
	std::cout << std::endl;
	std::cout << "======================================================" << std::endl;

	// Set our parameters
	std::string cfgfilename = "Configs/inputParameters.txt";
	SetParameters(cfgfilename, CUTS);

	// Make sure that we have the proper inputs.
	if (argc < 3)
	{
		std::cout << "ERROR: Please run this program while including";
		std::cout << "\n the initial and final entry numbers in the ";
		std::cout << "\n command line. [END PROGRAM]" << std::endl;
		return -1;
	}
	
	Int_t initEntry = std::atoi(argv[1]);
	Int_t finalEntry = std::atoi(argv[2]);
	
	// Create the ttbar selector & have it read the data.
	// So far we only have the one file, so it is hard-coded.
	TTbarSelector *ttbar = new TTbarSelector();
	TChain *ch = new TChain("Events");
	std::string line; std::ifstream myfile("files/ttbarData.txt");

	if (myfile.is_open())
	{
		Int_t nFiles = 0;
		while(getline(myfile,line)) {
			const char* cstr = line.c_str();
			ch->Add(cstr); ++nFiles;
		}
		myfile.close();
		std::cout << nFiles << " files added to TTbar analysis..." << std::endl;
	}
	else
	{
		std::cout << "Unable to open files/ttbarData.txt" << std::endl;
		return -1;
	}

	// Now that we have the data from the files, let's process.
	ch->Process(ttbar, "", finalEntry, initEntry);


	// Now move onto the Z+x analysis and add data to it.
	ZxSelector *zhf = new ZxSelector();
	TChain *ch2 = new TChain("Events");

	std::ifstream myfile2("files/dyData.txt");
	if(myfile2.is_open())
	{
		Int_t nFiles = 0;
		while(getline(myfile2,line)) {
			const char* cstr = line.c_str();
			ch2->Add(cstr); ++nFiles;
		}
		myfile2.close();
		std::cout << nFiles << " files added to Z+x analysis..." << std::endl;
	}
	else
	{
		std::cout << "Unable to open files/dyData.txt" << std::endl;
		return -1;
	}

	// Now that we have the data, let's process.
	ch2->Process(zhf, "", finalEntry, initEntry);

}
