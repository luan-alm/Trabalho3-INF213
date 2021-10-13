#include "Huffman.h"

HuffmanTree::HuffmanTree(int freqs[]) {
	MyPriorityQueue<PreHuffman> queue;
	for (int i=0; i<256; i++)
	{
		if (freqs[i] != 0) {
			PreHuffman p;
			p.node = new Nodo;
			p.node->elem = i;
			p.node->freq = freqs[i];
			queue.push(p);
		}

	}
	PreHuffman T;

	if (queue.size() == 1) //quando ha somente um caractere no arquivo
	{
		T.node = new Nodo; //cria um nodo com a mesma frequencia e faz esse caractere ser seu filho direito
		T.node->right = queue.top().node;
		T.node->freq = T.node->right->freq;
		queue.pop();
	}
	while (queue.size() > 1) //criando a arvore
	{
		T.node = new Nodo;
		T.node->right = queue.top().node;
		queue.pop();
		T.node->left = queue.top().node;
		queue.pop();
		T.node->freq = T.node->left->freq + T.node->right->freq;
		queue.push(T);
	}
	this->root = T.node; //aponta para a unica arvore restante
	string aux;
	mapeia(root, aux); //atribui os codigos
}

void HuffmanTree::mapeia(Nodo *root, string codigo) { //funcao responsavel por mapear cada caractere em seu devido caminho (strings de 1's e 0's)
	if (root == NULL)
		return;
	if (root->right == NULL && root->left == NULL) //chegamos em uma folha
		codigos.insert(make_pair(root->elem,codigo)); 
	mapeia(root->right, codigo + '1');
	mapeia(root->left, codigo + '0');
}

HuffmanTree::HuffmanTree(const HuffmanTree &other) {
	root = NULL;
	*this = other;
}

Nodo* HuffmanTree::copy(Nodo *root) {
	if (root == NULL)
		return NULL;
	Nodo *el = new Nodo;
	el->elem = root->elem;
	el->right = copy(el->right);
	el->left = copy(el->left);
	return el;
}

HuffmanTree& HuffmanTree::operator=(const HuffmanTree &other) {
	if (this == &other) return *this;
	destroy();
	root = copy(other.root);
	return *this;
}

void HuffmanTree::destroy(Nodo *root) {
	if (root == NULL)
		return;
	destroy(root->left);
	destroy(root->right);
	delete root;
}

void HuffmanTree::destroy() {
	destroy(root);
}

void HuffmanTree::comprimir(MyVec<bool> &out, const MyVec<char> &in) {
	for (int i=0; i<in.size(); i++)
	{
		string cod = codigos[in[i]]; //pegue o codigo(caminho) do caractere
		for (int j=0; j<cod.size(); j++) //atribua a out
		{
			if (cod[j] == '0')
				out.push_back(0);
			else if (cod[j] == '1')
				out.push_back(1);
		}
	}
}

void HuffmanTree::descomprimir(MyVec<char> &out, const MyVec<bool> &in) const {
	Nodo *ptr = root; //comecamos na raiz
	for (int i=0; i<in.size(); i++)
	{
		if (in[i] == true) //se o booleano for 1, va para a direita
			ptr = ptr->right;
		else
			ptr = ptr->left; //senao, para a esquerda

		if (ptr->right == NULL && ptr->left == NULL) //folha
		{
			out.push_back(ptr->elem); //coloque o elemento no vetor de saida
			ptr = root; // volta a raiz para ir ao proximo caractere
		}
	}
}