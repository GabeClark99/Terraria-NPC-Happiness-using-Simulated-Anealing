/*
	Gabriel Clark
	Terraria Optimal NPC Happiness using simulated Annealing
	May 2020
*/


#include "npc.hpp"
#include "group.hpp"

#include <iostream>
#include <time.h>

using std::cout;
using std::endl;


void Initialize(vector<Group>* groupsVec, vector<NPC>* homelessNpcs);

void Randomize(vector<Group>* groupsVec, vector<NPC>* homelessNpcs);

void Shuffle(vector<Group>* groupsVec);

void DisplayVersion(vector<Group> currentVersion);

void DisplayHomelessNpcs(vector<NPC> homelessNpcs);

int CalculateTotalScore(vector<Group> groupVec);

int main() 
{
	Group mastergroup;
	mastergroup.MakeListFromFile("npcs.tsv");
	
	vector<NPC> npcList = mastergroup.GetNpcList();
	vector<NPC> homelessNpcs = npcList;
	
	vector<Group> groupsVec;
	
	Initialize(&groupsVec, &homelessNpcs);
	
	//cout << "Initialization: "; DisplayVersion(groupsVec); cout << endl << endl;
	
	//cout << "Homeless NPCs: "; DisplayHomelessNpcs(homelessNpcs); cout << endl << endl;
	
	Randomize(&groupsVec, &homelessNpcs);
	
	cout << "Randomization: "; DisplayVersion(groupsVec); cout << endl << endl;
	
	//cout << "Homeless NPCs: "; DisplayHomelessNpcs(homelessNpcs); cout << endl << endl;
	
	Shuffle(&groupsVec);
	
	return 0;
}

void Initialize(vector<Group>* groupsVec, vector<NPC>* homelessNpcs)
{
	srand( time(NULL) );
	
	// creating a group, setting the biome, and adding it to the groups vector
	Group forrest; forrest.SetBiome("forrest"); groupsVec->push_back(forrest);
	Group desert; desert.SetBiome("desert"); groupsVec->push_back(desert);
	Group jungle; jungle.SetBiome("jungle"); groupsVec->push_back(jungle);
	Group snow; snow.SetBiome("snow"); groupsVec->push_back(snow);
	Group ocean; ocean.SetBiome("ocean"); groupsVec->push_back(ocean);
	Group hallow; hallow.SetBiome("hallow"); groupsVec->push_back(hallow);
	Group underground; underground.SetBiome("underground"); groupsVec->push_back(underground);
	
	
	// fill every forrest with one random npc each
	for(int i = 0; i < 7; ++i)
	{
		int num = rand() % (25 - i);
		groupsVec->at(i).AddNpc( homelessNpcs->at(num) );
		homelessNpcs->erase(homelessNpcs->begin() + num); // removing from list of homeless npcs
	}
}

void Randomize(vector<Group>* groupsVec, vector<NPC>* homelessNpcs)
{
	int groupNum;
	int npcNum;
	
	while(homelessNpcs->size() > 0)
	{
		groupNum = rand() % 7;
		npcNum = rand() % homelessNpcs->size();
		
		groupsVec->at(groupNum).AddNpc( homelessNpcs->at(npcNum) );
		homelessNpcs->erase(homelessNpcs->begin() + npcNum);
	}
}

void Shuffle(vector<Group>* groupsVec)
{
	int moves = 26;
	
	vector<Group>* tempVec = new vector<Group>;
	*tempVec = *groupsVec;
	
	for(int i = 0; i < moves; ++i)
	{
		int randomGroup1 = rand() % 7;
		// keep going until you find a group with more than one npc to steal from
		while(groupsVec->at(randomGroup1).GetNumberOfNpcs() <= 1)
		{
			randomGroup1 = rand() % 7;
		}
		
		int randomPosition1 = rand() % groupsVec->at(randomGroup1).GetNumberOfNpcs();
		
		int randomGroup2 = rand() % 7;
		
		// store it
		NPC npc = groupsVec->at(randomGroup1).GetNpc(randomPosition1);
		
		// place it
		groupsVec->at(randomGroup2).AddNpc(npc);
		
		// remove it
		groupsVec->at(randomGroup1).RemoveNPC(randomPosition1);	
	}
	
	cout << "new groupsVec: "; DisplayVersion(*groupsVec); cout << endl;
	
	
	int oldScore = CalculateTotalScore(*groupsVec);
	int newScore = CalculateTotalScore(*tempVec);
	
	cout << "old score: " << oldScore << ", new score: " << newScore << endl;
	
	if(newScore > oldScore)
	{
		cout << "new score is better, setting to main" << endl;
		*groupsVec = *tempVec;
	}
	else
	{
		cout << "old score is better, discarding new version" << endl;
	}
	
	delete tempVec;
	return;
}

void DisplayVersion(vector<Group> currentVersion)
{
	for(int i = 0; i < currentVersion.size(); ++i)
	{
		vector<NPC> npcList = currentVersion.at(i).GetNpcList();
		
		for(int j = 0; j < npcList.size(); ++j)
		{
			cout << npcList.at(j).GetName();
			
			if( j < npcList.size() - 1 )
			{
				cout << ",";
			}
		}
		
		if(i < currentVersion.size() - 1)
		{
			cout << " - ";
		}
	}
}

void DisplayHomelessNpcs(vector<NPC> homelessNpcs)
{
	for(int i = 0; i < homelessNpcs.size(); ++i)
	{
		cout << homelessNpcs.at(i).GetName();
		
		if(i < homelessNpcs.size() - 1)
		{
			cout << ",";
		}
	}
}

int CalculateTotalScore(vector<Group> groupVec)
{
	int totalScore = 0;
	
	for(int i = 0; i < groupVec.size(); ++i)
	{
		totalScore += groupVec.at(i).GetScore();
	}
	
	return totalScore;
}




