// PFA.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "Solver.h"
#include <iostream>
#include <fstream>

int main()
{
	std::vector<Stringer> PresetStrigners;
	std::vector<Skin> PresetSkins;

	Material Al = Material(2.78, 72400000000, 0.33);
	Material St = Material(7.8, 210000000000, 0.3);

	//Ugly Hardcoding
	PresetStrigners.push_back(Stringer(0.02, 0.00015, Al));
	PresetStrigners.push_back(Stringer(0.02, 0.0002, Al));
	PresetStrigners.push_back(Stringer(0.015, 0.0001, Al));
	PresetStrigners.push_back(Stringer(0.015, 0.00015, Al));
	PresetStrigners.push_back(Stringer(0.015, 0.00015, St));
	PresetStrigners.push_back(Stringer(0.015, 0.0002, St));

	//More of that
	PresetSkins.push_back(Skin(0.0008, Al));
	PresetSkins.push_back(Skin(0.001, Al));
	PresetSkins.push_back(Skin(0.0012, Al));



	auto pnl = Panel();
	pnl.sk = PresetSkins[2];
	for (int i = 0; i < 7; i++) pnl.stringers.push_back(PresetStrigners[1]);
	std::vector<double> Dels;
	for (int i = 0; i < 30000; i++) Dels.push_back(Solver(&pnl, (double)i).del);

	std::ofstream myfile;
	myfile.open("Results.csv");

	for (int i = 0; i < Dels.size(); i++) myfile << Dels[i] << "; ";
	myfile.close();
}

