
#include "group.hpp"
#include "npc.hpp"
#include <iostream>
#include <vector>
#include <fstream> // file input
#include <sstream> // extracting data from raw line data

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::istringstream;

Group::Group() 
{
	this->score = 0;
}

Group::Group(vector<NPC> npcVec) 
{ 
	this->score = 0;
	this->npcList = npcVec;
}



void Group::CalculateNewScore()
{	
	this->score = 0;
	
//cout << "\t\tCalculating New Score for " << this->biome << endl;
//cout << "\t\tNumber of NPCs: " << this->GetNumberOfNpcs() << endl;
	
	for(int i = 0; i < npcList.size(); ++i) // for each npc...
	{
//cout << "\t\t\tnpc: " << npcList.at(i).GetName() << endl;
		// biome preferences section
//cout << "\t\t\tBiome Preferences..." << endl;
		if(npcList.at(i).DoesBiomeLike(this->biome))
		{
//cout << "\t\t\t\tlikes, +1" << endl;
			score += likesScore;
		}
		if(npcList.at(i).DoesBiomeDislike(this->biome))
		{
//cout << "\t\t\t\tdislikes, -1" << endl;
			score += dislikesScore;
		}
		
		// neighbors preferences section
//cout << "\t\t\tNeighbor Preferences..." << endl;
		for(int j = i + 1; j < npcList.size(); ++j) // for every other npc...
		{
			if( npcList.at(i).DoesLove( npcList.at(j) ) ) // if i loves j...
			{
//cout << "\t\t\t\tloves, +2" << endl;
				score += lovesScore; 
			}
			if( npcList.at(i).DoesLike( npcList.at(j) ) ) // if i likes j...
			{
//cout << "\t\t\t\tlikes, +1" << endl;
				score += likesScore; 
			}
			if( npcList.at(i).DoesDislike( npcList.at(j) ) ) // if i dislikes j...
			{
//cout << "\t\t\t\tdislikes, -1" << endl;
				score += dislikesScore;
			}
			if( npcList.at(i).DoesHate( npcList.at(j) ) ) // if i hates j...
			{
//cout << "\t\t\t\thates, -2" << endl;
				score += hatesScore;
			}
			
			
			if( npcList.at(j).DoesLove( npcList.at(i) ) ) // if j loves i...
			{
//cout << "\t\t\tloves, +2" << endl;
				score += lovesScore;
			}
			if( npcList.at(j).DoesLike( npcList.at(i) ) ) // if j likes i...
			{
//cout << "\t\t\tlikes, +1" << endl;
				score += likesScore;
			}
			if( npcList.at(j).DoesDislike( npcList.at(i) ) ) // if j dislikes i...
			{
//cout << "\t\t\tdislikes, -1" << endl;
				score += dislikesScore;
			}
			if( npcList.at(j).DoesHate( npcList.at(i) ) ) // if j hates i...
			{
//cout << "\t\t\thates, -2" << endl;
				score += hatesScore;
			}
		}
	}
	
	return;
}

void Group::AddNpc(NPC newNpc) 
{
	this->npcList.push_back(newNpc);
}

void Group::AddNpc(NPC npc, int position)
{
	this->npcList.insert(this->npcList.begin() + position, npc);
}

void Group::RemoveLastNpc() { this->npcList.pop_back(); }

void Group::RemoveNPC(int position)
{
	this->npcList.erase(this->npcList.begin() + position);
}

void Group::RemoveAllNpcs()
{
	while(this->npcList.size() > 0)
	{
		this->RemoveLastNpc();
	}
	
	return;
}

// Goes through each line of file, pulling in relevent data to create npc instance, minus
// the neighbor preferences because it needs the npc variables for that before I set them.
// For instance, Angler likes Demolitionist, but Demolitionist gets created after Angler.
// Therefore, I cannot add teh Demolitionist variable to Angler likes becuase it does not exist yet.
// Due to this, it only records the names on the first pass, then goes through every npc and adds
// the right variable after every npc variable is created.
void Group::MakeListFromFile(string filename) // returns bool indicating the success of creating the list
{
//cout << "start of makelistfrom file" << endl;
	std::ifstream file;
	file.open(filename.c_str()); // converting to cstring to make function happy
	
	if( !file.is_open() ) // error checking for correct file name
	{
		return;
	}
	
	string lineRaw;
	
	getline(file, lineRaw); // skipping first header line
	getline(file, lineRaw); // skipping second header line
	while( getline(file, lineRaw) ) // get each line, stop when no more lines
	{
		vector<string> tokens;
		
		istringstream lineRawStream(lineRaw);
//cout << "\t lineRaw: " << lineRaw << endl;
		string token;
		while( getline(lineRawStream, token, '\t') ) // step through each token in lineRaw, splitting on tab character
		{
			tokens.push_back(token);
		}
	
// display contents of tokens vector	
/*cout << "tokens: ";
for(unsigned int i = 0; i < tokens.size(); ++i)
{
	cout << tokens.at(i) << " ";
}
cout << endl << endl;*/
		
		// create a new npc and populate its fields with data from line
		NPC* npc = new NPC();
		npc->SetName( tokens.at(0) );
		npc->SetBiomeLike( tokens.at(1) );
		npc->SetBiomeDislike( tokens.at(2) );
		npc->SetNpcLovesNames( tokens.at(3) );
//cout << tokens.at(0) << " loves set to: " << tokens.at(3) << endl;
		npc->SetNpcLikesNames( tokens.at(4) );
//cout << tokens.at(0) << " likes set to: " << tokens.at(4) << endl;
		npc->SetNpcDislikesNames( tokens.at(5) );
//cout << tokens.at(0) << " dislikes set to: " << tokens.at(5) << endl;
		npc->SetNpcHatesNames( tokens.at(6) );
//cout << tokens.at(0) << " hates set to: " << tokens.at(6) << endl << endl;
		
		this->AddNpc(*npc);
	}
	
// displays position in list and its corresponding npc
/*for(unsigned int i = 0; i < this->npcList.size(); ++i)
{
	cout << i << " " << this->npcList.at(i).GetName() << endl;
}
cout << endl;*/
	
	for(unsigned int i = 0; i < this->npcList.size(); ++i)
	{
//cout << "\ti: " << i << endl;
		string loves = this->npcList.at(i).GetNpcLovesNames();
		string likes = this->npcList.at(i).GetNpcLikesNames();
		string dislikes = this->npcList.at(i).GetNpcDislikesNames();
		string hates = this->npcList.at(i).GetNpcHatesNames();
//cout << "\tloves: " << loves << endl;
//cout << "\tlikes: " << likes << endl;
//cout << "\tdislikes: " << dislikes << endl;
//cout << "\thates: " << hates << endl << endl;
		istringstream lovesStream(loves);
		istringstream likesStream(likes);
		istringstream dislikesStream(dislikes);
		istringstream hatesStream(hates);
		
		string token;
//cout << "before preferences" << endl;
		vector<string> lovesTokens;
		while( getline(lovesStream, token, ',') ) // convert string of names into vector of names
		{
			lovesTokens.push_back(token);
		}
/*for(unsigned int j = 0; j < lovesTokens.size(); ++j)
{
	cout << "\tlovesTokens at " << j << ": \"" << lovesTokens.at(j) << "\"" << endl;
}*/
		for(unsigned int j = 0; j < lovesTokens.size(); ++j) // find npc name is attached to and add it to the list of loves
		{
//cout << "\t\tj: " << j << endl;
			if(lovesTokens.at(j) == "-") // means there are no loves, skip it
			{
//cout << "\t\tname is \"-\", continuing." << endl;
				continue;
			}
			
			NPC* npc = this->Find( lovesTokens.at(j) );
			if(npc != nullptr) // if it found an npc with that name...
			{
//cout << "\t\tfound an npc with that name at " << npc << endl;
				this->npcList.at(i).AddLove(npc);
			}
			else // if it didn't find an npc with that name...
			{
				cout << "---COULD NOT FIND NPC NAMED " << lovesTokens.at(j) << "---" << endl;
			}
		}
		
		vector<string> likesTokens;
		while( getline(likesStream, token, ',') )
		{
			likesTokens.push_back(token);
		}
		for(unsigned int j = 0; j < likesTokens.size(); ++j)
		{
			if(likesTokens.at(j) == "-")
			{
				continue;
			}
			
			NPC* npc = this->Find( likesTokens.at(j) );
			if(npc != nullptr)
			{
				this->npcList.at(i).AddLike(npc);
			}
			else
			{
				cout << "---COULD NOT FIND NPC NAMED " << likesTokens.at(j) << "---" << endl;
			}
		}
		
		vector<string> dislikesTokens;
		while( getline(dislikesStream, token, ',') )
		{
			dislikesTokens.push_back(token);
		}
		for(unsigned int j = 0; j < dislikesTokens.size(); ++j)
		{
			if(dislikesTokens.at(j) == "-")
			{
				continue;
			}
			
			NPC* npc = this->Find( dislikesTokens.at(j) );
			if(npc != nullptr)
			{
				this->npcList.at(i).AddDislike(npc);
			}
			else
			{
				cout << "---COULD NOT FIND NPC NAMED " << dislikesTokens.at(j) << "---" << endl;
			}
		}
		
		vector<string> hatesTokens;
		while( getline(hatesStream, token, ',') )
		{
			hatesTokens.push_back(token);
		}
		for(unsigned int j = 0; j < hatesTokens.size(); ++j)
		{
			if(hatesTokens.at(j) == "-")
			{
				continue;
			}
			
			NPC* npc = this->Find( hatesTokens.at(j) );
			if(npc != nullptr)
			{
				this->npcList.at(i).AddHate(npc);
			}
			else
			{
				cout << "---COULD NOT FIND NPC NAMED " << hatesTokens.at(j) << "---" << endl;
			}
		}
	}
//cout << "end of makelistfrom file" << endl;
	file.close();
	return;
}

NPC* Group::Find(string npcName)
{
	for(unsigned int k = 0; k < this->npcList.size(); ++k)
	{
//cout << "\t\t\tk: " << k << endl;
		if(this->npcList.at(k).GetName() == npcName)
		{
			return &( this->npcList.at(k) );
		}
	}
//cout << "\t\tcouldn't find it, returning nullptr." << endl;
	return nullptr;
}

vector<NPC> Group::GetNpcList() { return this->npcList; }

int Group::GetScore() 
{
	this->CalculateNewScore();
	return this->score; 
}

void Group::SetBiome(string biome) { this->biome = biome; }

string Group::GetBiome() { return this->biome; }

int Group::GetNumberOfNpcs() { return this->npcList.size(); }

NPC Group::GetNpc(int position) { return this->npcList.at(position); }





