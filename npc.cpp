
#include "npc.hpp"

NPC::NPC() {}

NPC::NPC(string name) { this->name = name;}

bool NPC::Find(NPC npc, vector<NPC> npcVec)
{
	for(int i = 0; i < npcVec.size(); ++i)
	{
		if(npcVec.at(i).GetName() == npc.GetName())
		{
			return true;
		}
	}
	
	return false;
}

void NPC::SetName(string name) { this->name = name; }
string NPC::GetName() { return this->name; }



void NPC::AddLove(NPC* npc) { this->npcLoves.push_back( *(npc) ); }

void NPC::AddLike(NPC* npc) { this->npcLikes.push_back( *(npc) ); }

void NPC::AddDislike(NPC* npc) { this->npcDislikes.push_back( *(npc) ); }

void NPC::AddHate(NPC* npc) { this->npcHates.push_back( *(npc) ); }



void NPC::SetNpcLovesNames(string names) { this->npcLovesNames = names; }
string NPC::GetNpcLovesNames() { return this->npcLovesNames; }

void NPC::SetNpcLikesNames(string names) { this->npcLikesNames = names; }
string NPC::GetNpcLikesNames() { return this->npcLikesNames; }

void NPC::SetNpcDislikesNames(string names) { this->npcDislikesNames = names; }
string NPC::GetNpcDislikesNames() { return this->npcDislikesNames; }

void NPC::SetNpcHatesNames(string names) {  this->npcHatesNames = names;  }
string NPC::GetNpcHatesNames() { return this->npcHatesNames; }



void NPC::SetBiomeLike(string biome) { this->biomeLike = biome; }

void NPC::SetBiomeDislike(string biome) { this->biomeDislike = biome; }


bool NPC::DoesLove(NPC npc)
{
	return this->Find(npc, this->npcLoves);
}

bool NPC::DoesLike(NPC npc)
{
	return this->Find(npc, this->npcLikes);
}

bool NPC::DoesDislike(NPC npc)
{
	return this->Find(npc, this->npcDislikes);
}

bool NPC::DoesHate(NPC npc)
{
	return this->Find(npc, this->npcHates);
}

bool NPC::DoesBiomeLike(string biome)
{
	if(this->biomeLike == biome)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool NPC::DoesBiomeDislike(string biome)
{
	if(this->biomeDislike == biome)
	{
		return true;
	}
	else
	{
		return false;
	}
}





