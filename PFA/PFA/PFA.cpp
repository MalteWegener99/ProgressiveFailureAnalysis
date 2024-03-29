// PFA.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "Solver.h"
#include <iostream>
#include <fstream>
#define MAXSTRIGNERS 8
#include <algorithm>

//-----------------------------------
//Edit your ceilings here
#define CEILIR 100000
#define CEILLA 100000
#define CEILCO 300000
//-----------------------------------

bool comp(const Resultv2 &lhs, const Resultv2 &rhs) { return lhs.mass < rhs.mass; }
void printer(std::vector<double> seed)
{
	std::cout << std::endl << seed[0] << std::endl;
	for (int i = 1; i < seed.size(); i+=4)
	{
		std::cout << seed[i] << "|" << seed[i + 1] << "|" << seed[i+2]<< "|" << seed[i + 3] << std::endl;
	}
}

int main()
{
	std::vector<Stringer> PresetStrigners;
	std::vector<Skin> PresetSkins;

	Material Al = Material(2.78*1000, 72400000000, 0.33);
	Material St = Material(7.8*1000, 210000000000, 0.3);
	Material Super = Material(1 * 1000, 210000000000, 0.3);

	//Ugly Hardcoding
	PresetStrigners.push_back(Stringer(0.02, 0.0015, Al));
	PresetStrigners.push_back(Stringer(0.02, 0.002, Al));
	PresetStrigners.push_back(Stringer(0.015, 0.001, Al));
	PresetStrigners.push_back(Stringer(0.015, 0.0015, Al));
	PresetStrigners.push_back(Stringer(0.010, 0.015, 0.0015, Super));
	//PresetStrigners.push_back(Stringer(0.015, 0.0015, St));
	//PresetStrigners.push_back(Stringer(0.015, 0.002, St));

	//More of that
	PresetSkins.push_back(Skin(0.0008, Al));
	PresetSkins.push_back(Skin(0.001, Al));
	PresetSkins.push_back(Skin(0.0012, Al));

	//Lets do some sick analysis of many pussybilities
	std::vector<Resultv2> resses{};

	auto pnl = Panel();
	//All skins
	for (int i = 0; i < PresetSkins.size(); i++)
	{
		for (int j = 2; j < MAXSTRIGNERS; j++)
		{
			if (j == 2)
			{
				for (int k = 0; k < PresetStrigners.size(); k++)
				{
					pnl = Panel();
					pnl.sk = PresetSkins[i];
					pnl.stringers.push_back(PresetStrigners[k]);
					pnl.stringers.push_back(PresetStrigners[k]);
					resses.push_back(Solverv3(&pnl));
				}
			}

			else if (j == 3 | j == 4)
			{
				for (int k = 0; k < PresetStrigners.size(); k++)
				{
					for (int p = 0; p < PresetStrigners.size(); p++)
					{
						pnl = Panel();
						pnl.sk = PresetSkins[i];
						pnl.stringers.push_back(PresetStrigners[k]);
						pnl.stringers.push_back(PresetStrigners[k]);
						pnl.stringers.push_back(PresetStrigners[p]);
						if( j == 4) pnl.stringers.push_back(PresetStrigners[p]);
						resses.push_back(Solverv3(&pnl));
					}
				}
			}

			else if (j == 5 | j == 6)
			{
				for (int k = 0; k < PresetStrigners.size(); k++)
				{
					for (int p = 0; p < PresetStrigners.size(); p++)
					{
						for (int o = 0; o < PresetStrigners.size(); o++)
						{
							pnl = Panel();
							pnl.sk = PresetSkins[i];
							pnl.stringers.push_back(PresetStrigners[k]);
							pnl.stringers.push_back(PresetStrigners[k]);
							pnl.stringers.push_back(PresetStrigners[p]);
							pnl.stringers.push_back(PresetStrigners[p]);
							pnl.stringers.push_back(PresetStrigners[o]);
							if(j==6)pnl.stringers.push_back(PresetStrigners[o]);
							resses.push_back(Solverv3(&pnl));
						}
					}
				}
			}

			else if (j == 7 | j == 8)
			{
				for (int k = 0; k < PresetStrigners.size(); k++)
				{
					for (int p = 0; p < PresetStrigners.size(); p++)
					{
						for (int o = 0; o < PresetStrigners.size(); o++)
						{
							for (int u = 0; u < PresetStrigners.size(); u ++ )
							{
								pnl = Panel();
								pnl.sk = PresetSkins[i];
								pnl.stringers.push_back(PresetStrigners[k]);
								pnl.stringers.push_back(PresetStrigners[k]);
								pnl.stringers.push_back(PresetStrigners[p]);
								pnl.stringers.push_back(PresetStrigners[p]);
								pnl.stringers.push_back(PresetStrigners[o]);
								pnl.stringers.push_back(PresetStrigners[o]);
								pnl.stringers.push_back(PresetStrigners[u]);
								if (j == 8)pnl.stringers.push_back(PresetStrigners[u]);
								resses.push_back(Solverv3(&pnl));
							}
						}
					}
				}
			}
		}
	}
	//Wow that was ugly

	std::vector<Resultv2> realshit{};

	for (int i = 0; i < resses.size(); i++) {
		auto tmp = &resses[i];
		if (tmp->works && tmp->irbckl < CEILIR && tmp->lateral < CEILLA&&tmp->coulumn < CEILCO)
		{
			realshit.push_back(resses[i]);
		}
	}
	std::sort(realshit.begin(), realshit.end(), comp);
	std::cout << realshit[0].mass << "|||||" << realshit[realshit.size()-1].mass<<"|||||"<<resses.size();
	printer(realshit[0].seed);
	
	auto pnl2 = Panel();
	pnl2.sk = PresetSkins[0];
	for (int i = 0; i < 7; i++) pnl2.stringers.push_back(PresetStrigners[0]);
	realshit[0] = Solverv3(&pnl2);

	std::cout << realshit[0].irbckl << std::endl;
	std::cout << realshit[0].lateral << std::endl;
	std::cout << realshit[0].coulumn << std::endl;
}


