#include <iostream>
#include<string>
#include <bitset>
#include<fstream>
#include <queue>
#include <map>

using namespace std;

typedef vector<bool> HuffCode;
typedef map<char, HuffCode> HuffCodeMap;

//generic node class that carried the cumulative
//frequencies of all nodes to the left and right (f)
class INode
{
public:
	const int f;
	INode(int f) : f(f) {}

	virtual ~INode() {}

};
//Internal node that connects either leaf nodes or other internal nodes
class InternalNode : public INode
{
public:
	INode  *const left;
	INode  *const right;
	InternalNode(INode* c0, INode* c1) : INode(c0->f + c1->f), left(c0), right(c1) {}

};
//Leaf class that also contains a character
class LeafNode : public INode
{
public:
	char c;
	LeafNode(int f, char c) : INode(f), c(c) {}
};
//compare two nodes to arrange priority 
struct CompareNode {
	bool operator()(INode* node1, INode* node2) {
		return node1->f > node2->f;
	}
};

INode* BuildHuffmanTree(const int(&frequencies)[256])
{
	priority_queue<INode*, vector<INode*>, CompareNode> trees;

	for (int i = 0; i < 256; ++i)
	{
		if (frequencies[i] != 0)
			trees.push(new LeafNode(frequencies[i], (char)i));
	}
	// Extract the two minimum freq items 
	while (trees.size() > 1)
	{
		INode* childR = trees.top();
		trees.pop();

		INode* childL = trees.top();
		trees.pop();

		INode* parent = new InternalNode(childR, childL);
		trees.push(parent);
	}
	return trees.top(); //Root Node  we can traverse the whole tree
}

void GenerateCodes(const INode* node, const HuffCode& prefix, HuffCodeMap& outCodes)
{

	if (const LeafNode* leaf = dynamic_cast<const LeafNode*>(node))
	{
		outCodes[leaf->c] = prefix;
	}
	else if (const InternalNode* internal = dynamic_cast<const InternalNode*>(node))
	{
		HuffCode leftPrefix = prefix;
		leftPrefix.push_back(0);//leftchilds get 0
		GenerateCodes(internal->left, leftPrefix, outCodes);

		HuffCode rightPrefix = prefix;
		rightPrefix.push_back(1);//rightchilds get 1
		GenerateCodes(internal->right, rightPrefix, outCodes);
	}
}

int main()
{
	ifstream fin("input.txt");
	ofstream fout("ascii.txt");
	string input, temp;

	while (!fin.eof()) { //Gather data from input file	
		getline(fin, temp); //extract to string
		input.append(temp + "\n"); //temp to input
	}
	cout << input << endl;
	for (int i = 0; i < input.length(); i++) { //convert input to ascii
		fout << bitset<8>((int)input[i]).to_string() << " ";
	}
	int frequencies[256] = { 0 }; //arrange frequencies and print on screen
	const char* ptr = input.c_str();
	while (*ptr != '\0')
		++frequencies[*ptr++];
	for (int i = 0; i < 256; i++) {
		if (frequencies[i] != 0)
			cout << (char)i << " " << frequencies[i] << endl;
	}

	INode *root = BuildHuffmanTree(frequencies);//build a huffman tree
	HuffCodeMap codes;
	GenerateCodes(root, HuffCode(), codes); //generate code
	delete root;
	fout.close();
	fout.open("huffman.txt");
	for (int q = 0; q < input.length(); q++)//encode huffman
	{
		for (HuffCodeMap::iterator j = codes.begin(); j != codes.end(); j++)
		{
			if (j->first == input[q]) //getting right code for character
			{
				int size = j->second.size();
				vector <int> coded(size);
				copy(j->second.begin(), j->second.end(), coded.begin());
				for (vector<int>::iterator k = coded.begin(); k != coded.end(); k++) {
					fout << *k; //file output
				}
				fout << " "; //seperater
			}
		}
	}

	system("PAUSE");
	return 0;
}


