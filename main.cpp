#include <iostream>
#include "MyVecNewIterator.h"
#include "Huffman.h"
#include <fstream>
#include <bitset>
using namespace std;


int main(int argc, char **argv) {
	if (argc != 4) 
        return 0;
    string tipo = argv[1];
    ifstream entrada(argv[2]);
    ofstream saida(argv[3]);
    MyVec<char> in;
    MyVec<bool> out;
    int frequencias[256] = {0};
    int num_bits; //guarda o numero total de bits
    if (tipo == "c")
    {
        unsigned char caractere;
        while(true)
        {
            caractere = entrada.get();
            if (entrada.eof()) 
                break;
            in.push_back(caractere); //pega os caracteres da entrada
            frequencias[caractere]++;//aumenta a frequencia
        }
        entrada.close();
        HuffmanTree Huff(frequencias); //cria a arvore
        Huff.comprimir(out,in);

        num_bits = out.size();
        saida.write(reinterpret_cast<char *>(&num_bits), sizeof(int)); //grava o numero de bits
        saida.write(reinterpret_cast<char *>(frequencias), sizeof(int)*256); //depois as frequencias

        string aux; //esta string armazena de 8 em 8 bits os booleanos do vetor out
        for (int i=0; i<out.size(); i++)
        {
            if (out[i] == 0)
                aux.push_back('0');
            else
                aux.push_back('1');
            
            if (i == out.size()-1 && aux.size() < 8) //se a ultima string ficar com menos de 8 bits, completa com 0's ate ficar com 8
            {
                while (aux.size()<8)
                    aux.push_back('0');

            }
            if (aux.size() == 8)
            {
                int num = stoi(aux, nullptr, 2); //pega os 8 bits da string e transforma-os de binario para um int decimal
                unsigned char caract = num; //trunca o int para char (1 byte)
                saida.write(reinterpret_cast<char *>(&caract),sizeof(char)); //grava o char na saida
                aux.clear();

            }
        }
        saida.close();

    } else if (tipo == "d") {
        entrada.read(reinterpret_cast<char *>(&num_bits),sizeof(int)); //le o numero de bits
        entrada.read(reinterpret_cast<char *>(frequencias),sizeof(int)*256); //le as frequencias
    
        int num_bytes = num_bits/8;
        int resto = num_bits % 8; //numero de bits que "sobram" ao agrupar conjuntos de bytes
        if (resto != 0)
            num_bytes++; //necessario um byte a mais para os bits que sobram

        for (int i=0; i<num_bytes; i++)
        {
            unsigned char caractere;
            entrada.read(reinterpret_cast<char *>(&caractere),sizeof(char));
            string byte = bitset<8>(caractere).to_string(); //converte o caractere para um numero binario em uma string de 8 bits
            for (int j=0; j<byte.size(); j++)
            {
                if (i == num_bytes-1 && resto != 0) //se estiver no ultimo byte e houver "sobras"
                {
                    if (j >= resto) //para nao copiar os 0's que foram adicionados no final na parte de comprimir
                        break;
                }
                if (byte[j] == '0')
                    out.push_back(0);
                else if (byte[j] == '1')
                    out.push_back(1);
            }
        }
        HuffmanTree Huff(frequencias); //agora, cria a arvore
        Huff.descomprimir(in,out);
        entrada.close();
        for (int i=0; i<in.size(); i++)
            saida.write(reinterpret_cast<char *>(&in[i]),sizeof(char)); //grave o vetor no arquivo
    }
    return 0;
}
