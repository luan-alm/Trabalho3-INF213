#ifndef Huffman_H
#define Huffman_H
#include "MyPriorityQueue.h"	
#include "MyMap.h"
#include <algorithm>
#include <string>
using namespace std;

class Nodo {
public:
	Nodo() : elem(char()), left(NULL), right(NULL), freq(int()) {}
	Nodo *left, *right;
	char elem;
	int freq;
};

class PreHuffman { //classe auxiliar, para ajudar a montar a arvore
	public:
	PreHuffman() {node = NULL;};
	Nodo *node;

	//operadores para a fila de prioridade
	bool operator > (const PreHuffman &other) {
		return (this->node->freq < other.node->freq);
	}

	bool operator < (const PreHuffman &other) {
		return (this->node->freq > other.node->freq);
	}
};

class HuffmanTree {
private:
	Nodo *root;
	MyMap<char,string> codigos; //codigos dos caminhos para as folhas
	
	void mapeia(Nodo *root, string codigo);
	void destroy();
	void destroy(Nodo *root);
	Nodo * copy(Nodo *root);

public:
	HuffmanTree(int freqs[]);
	HuffmanTree() {root = NULL;}

	HuffmanTree(const HuffmanTree &other); //construtor de copia
	HuffmanTree & operator=(const HuffmanTree &other); //operador de atribuicao
	~HuffmanTree(){ destroy(); }; //destrutor

	void comprimir(MyVec<bool> &out, const MyVec<char> &in);
	void descomprimir(MyVec<char> &out, const MyVec<bool> &in) const;
};

#endif