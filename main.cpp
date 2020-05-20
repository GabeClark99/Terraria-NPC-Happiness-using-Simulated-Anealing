/*
	Gabriel Clark
	Terraria Optimal NPC Happiness using simulated Annealing
	May 2020
*/


#include "npc.hpp"
#include "group.hpp"

#include <iostream>
#include <time.h> // used for seeding randomness
#include <math.h> // exp()

using std::cout;
using std::endl;

void Anneal(/*vector<Group>* groupsVec,*/ double initialtemp, double coolFactor, int time, vector<NPC> npcList);

void Initialize(vector<Group>* groupsVec/*, vector<NPC>* homelessNpcs*/);

void EvictNpcs(vector<Group>* groupsVec);

void Randomize(vector<Group>* groupsVec, vector<NPC> npcList);

void HeatAtTempForTime(vector<Group>* groupsVec, vector<Group>* bestGroupsVec, vector<NPC> npcList, int temp, int time);

void UpdateVersion(vector<Group>* groupsVec, vector<Group>* newGroupsVec, int* bestScore, double temp);

void DisplayVersion(vector<Group> currentVersion);

void DisplayHomelessNpcs(vector<NPC> homelessNpcs);

int CalculateTotalScore(vector<Group> groupVec);

void MoveNpc(vector<Group>* newGroupsVec);


int main() 
{
	Group mastergroup;
	mastergroup.MakeListFromFile("npcs.tsv");
	
	vector<NPC> npcList = mastergroup.GetNpcList();
	//vector<NPC> homelessNpcs = npcList;
	
	vector<Group> groupsVec;
	
	
	Anneal(30, 0.99, 100, npcList);
	
	return 0;
}

void Anneal(double initialtemp, double coolFactor, int time, vector<NPC> npcList)
{
	vector<Group> groupsVec;
	Initialize(&groupsVec);
	
	vector<Group> bestGroupsVec = groupsVec;
	
	for(double temp = initialtemp; temp > 0.001; temp = coolFactor * temp)
	{
		cout << endl << "---TEMP: " << temp << "---" << endl << endl;
		
		HeatAtTempForTime(&groupsVec, &bestGroupsVec, npcList, temp, time);
	}
	
	cout << "Best Version: " << endl; DisplayVersion(bestGroupsVec); cout << endl;
	cout << "Best Score: " << CalculateTotalScore(bestGroupsVec); cout << endl;
	
	
	return;
}

void Initialize(vector<Group>* groupsVec/*, vector<NPC>* homelessNpcs*/)
{
	srand( time(NULL) );
	
	// creating a group, setting the biome, and adding it to the groups vector
	Group forrest; forrest.SetBiome("Forest"); groupsVec->push_back(forrest);
	Group desert; desert.SetBiome("Desert"); groupsVec->push_back(desert);
	Group jungle; jungle.SetBiome("Jungle"); groupsVec->push_back(jungle);
	Group snow; snow.SetBiome("Snow"); groupsVec->push_back(snow);
	Group ocean; ocean.SetBiome("Ocean"); groupsVec->push_back(ocean);
	Group hallow; hallow.SetBiome("Hallow"); groupsVec->push_back(hallow);
	Group underground; underground.SetBiome("Underground"); groupsVec->push_back(underground);
	
	///DisplayHomelessNpcs(*homelessNpcs); cout << endl;
	//cout << homelessNpcs->size() << endl;
	
	/*// fill every group with one random npc each
	int numNpcs = homelessNpcs->size();
	for(int i = 0; i < 7 && i < numNpcs; ++i)
	{
		//cout << "i: " << i << endl;
		
		int num = rand() % ( homelessNpcs->size() ); // picks random npc from remaining ones	
		groupsVec->at(i).AddNpc( homelessNpcs->at(num) );
		homelessNpcs->erase(homelessNpcs->begin() + num); // removing from list of homeless npcs
	}*/
	
	//cout << "---INITIALIZATION COMPLETE---" << endl << endl;
}

void HeatAtTempForTime(vector<Group>* groupsVec, vector<Group>* bestGroupsVec, vector<NPC> npcList, int temp, int time)
{
	int bestScore = CalculateTotalScore(*bestGroupsVec);
	
	for(int i = 0; i < time; ++i)
	{
		//cout << "Best Version: " << endl; DisplayVersion(*bestGroupsVec);
		
		Randomize(groupsVec, npcList);
		
		//cout << "Current Version: " << endl; DisplayVersion(*groupsVec);
		
		UpdateVersion(groupsVec, bestGroupsVec, &bestScore, temp);
		
		EvictNpcs(groupsVec);
	}
	
	return;
}

// goes through npcList in descending order and adds each npc to a random group
void Randomize(vector<Group>* groupsVec, vector<NPC> npcList)
{
	while(npcList.size() > 0)
	{
		int groupNum = rand() % 7;
		//int npcNum = rand() % npcList.size();
		
		groupsVec->at(groupNum).AddNpc( npcList.back() );
		npcList.pop_back();
	}
	
	return;
}

void UpdateVersion(vector<Group>* groupsVec, vector<Group>* bestGroupsVec, int* bestScore, double temp)
{
	//int oldScore = CalculateTotalScore(*groupsVec);
	int newScore = CalculateTotalScore(*groupsVec);
	
	//cout << "\tbest score: " << *bestScore << ", new score: " << newScore << endl;
	
	if(newScore > *bestScore)
	{
		//cout << "\tnew score is better, updating best version" << endl << endl;
		*bestGroupsVec = *groupsVec;
		*bestScore = newScore;
		
		return ;
	}
	else
	{
		//cout << "\told score is better, ";
		
		double p = exp( -1*abs(newScore - *bestScore) / temp );
		double r = rand() / ( (double)RAND_MAX ); // will generate number on [0,1]
		
		if(r < p)
		{
			//cout << "but we're taking the new score anyway." << endl;
			*bestGroupsVec = *groupsVec;
			*bestScore = newScore;
			
			//cout << "p: " << p << " > r: " << r << endl << endl;
			
			return;
		}
		else
		{
			//cout << "discarding new version." << endl;
			
			//cout << "\tp: " << p << " <= r: " << r << endl << endl;
			
			return;
		}
		
		
	}
}

// removes all npcs from each group in groupsVec
void EvictNpcs(vector<Group>* groupsVec)
{
	for(int i = 0; i < groupsVec->size(); ++i)
	{
		groupsVec->at(i).RemoveAllNpcs();
	}
	
	return;
}

void DisplayVersion(vector<Group> currentVersion)
{
	for(int i = 0; i < currentVersion.size(); ++i)
	{
		cout << "\t" << currentVersion.at(i).GetBiome() << ": ";
		
		vector<NPC> npcList = currentVersion.at(i).GetNpcList();
		
		for(int j = 0; j < npcList.size(); ++j)
		{
			cout << npcList.at(j).GetName();
			
			if( j < npcList.size() - 1 )
			{
				cout << ", ";
			}
		}
		
		if(i < currentVersion.size() - 1)
		{
			cout << /*" - "*/ endl;
		}
	}
	
	cout << endl << endl;
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

void MoveNpc(vector<Group>* newGroupsVec)
{
	int randomGroup1 = rand() % 7;
	// keep going until you find a group with more than one npc to steal from
	/*while(newGroupsVec->at(randomGroup1).GetNumberOfNpcs() <= 1)
	{
		randomGroup1 = rand() % 7;
	}*/
	
	// Can't take an npc from a group if it doesn't have any
	while(newGroupsVec->at(randomGroup1).GetNumberOfNpcs() < 1)
	{
		randomGroup1 = rand() % 7;
	}
	
	int randomPosition1 = rand() % newGroupsVec->at(randomGroup1).GetNumberOfNpcs();
	
	int randomGroup2 = rand() % 7;
	
	// store it
	NPC npc = newGroupsVec->at(randomGroup1).GetNpc(randomPosition1);
	
	// place it
	newGroupsVec->at(randomGroup2).AddNpc(npc);
	
	// remove it
	newGroupsVec->at(randomGroup1).RemoveNPC(randomPosition1);	
}






