
#ifndef GROUP_HPP
#define GROUP_HPP

#include "npc.hpp"
#include <vector>
#include <string>

using std::vector;

const int lovesScore = 2;
const int likesScore = 1;
const int dislikesScore = -1;
const int hatesScore = -2;

class Group
{
	private:
		vector<NPC> npcList;
		int score;
		string biome;
		
		void CalculateNewScore();
	
	public:
		Group();
		Group(vector<NPC> npcVec);
		
		void MakeListFromFile(string filename);
		
		void AddNpc(NPC newNpc);
		void RemoveLastNpc();
		NPC* Find(string npcName);
		
		vector<NPC> GetNpcList();
		int GetScore();
		void SetBiome(string biome);
};

#endif
