//farei uma explicaçao bem objetiva, focando somente na logica de resoluçao do problema
//eu acabei fazendo merda pq chegava um ponto que a porra das contas passavam a incluir numeros
//com 16 bytes e eu nem sequer cheguei a conferir qual era o limite do tipo long double, por isso,
//por isso tomei a soluçao mais burra possivel que era desenvolver uma calculadora mais potente
//usando string pra alocar os numeros e funçoes especificas pra realizar as operaçoes
//por sorte eu estava com um projeto parado envolvendo essa ideia e foi soh recuperar e
//fazer algumas novas funçoes
//nao vou nem explicar a logica das funçoes pq daria um trampo desgraçado
//talvez algum dia eu explique essa parte especificamente

//Outro detalhe importante eh que eu acabei me esquecendo de preocupar com a agilidade do programa
//ja que eu nao imaginava que daria mais de 500 linhas, por isso, tem uma porrada de variavel alocada de forma errada
//e depois da execuçao do programa, fica uma bucetada de lixo na memoria, por isso, ele vai deixar seu pc travando
//para porra. Outra coisa, eh que ele precisa estar dedicado, ou seja, se vc nao disponibilizar recurso suficiente da maquina
//ele vai morrer por estrapolar os 3 segundos limites de resposta
//se vc der uma olhada, com ctz vai conseguir melhorar essa joça, eh um dos piores codigos que u ja fiz
//so perde para o bomberman de 800 linhas no main

//Outra coisa, provavelmente vai dar problema de compatibilidade pq tem gambiarra pra caralho, entao, paciencia

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define TAM 10000
//funçao somente pra converter um char pra um inteiro, frescura
int chartoint(char num){
   return num-48;
}

//funçao para tirar os 0s da frente do numero pq a operaçao de subtraçao deixa alguns 0s na frente do resultado
char *tirazeros(char *num){
   char *troca;
   troca=malloc(TAM*sizeof(char));
   int start=0;
   int j=0;
   for(int i=0;i<strlen(num); i++){
      if(i==strlen(num)-1){
         troca[j]=num[i];
         break;
      }
      else if(num[i]=='0' && start==0){
         continue;
      }
      else if(start==1){
         troca[j]=num[i];
         j++;
      }
      else if(num[i]!='0' && start==0){
         troca[j]=num[i];
         j++;
         start=1;
      }
   }
   return troca;
}

//funçao pra inverter uma string, eu sei q existe a nativa, mas por algum motivo
//minha string.h parece estar incompleta
char *strrev (char *str)
{
   char *copia;
   copia=(char *)malloc(TAM*sizeof(char));
   int u=0;
   int i;
   for(i=strlen(str)-1; i>=0; i--){
      copia[u]=str[i];
      u++;
   }
   copia[u+1]='\0';

   return copia;
}
//soma de numeros decimais, a ideia do projeto inicial era ter 2 funçoes pra decimais e naturais
//so o nome msm
//num1=359
//num2=275
//primeiro se inverte os numeros, ficando: 953 e 572
//agora soma-se cada casa com a outra da mesma posicao:
//resto=0
//num1[0]+num2[0]+resto = 2+3=5 --> valpas[0]=5 e resto=0
//num1[1]+num2[1]+resto = 7+5=12 --> valpas[1]=2 e resto=1
//num1[2]+num2[2]+resto = 5+9+1=15 --> valpas[2]=5 e resto=1
//fim do loop, concatena-se o resto 1 ao restultado:1525
//inverte o resultado: 5251
char *somadec(char *num111, char *num222){
   char copnum1[TAM], copnum2[TAM];
   int valpas;
   char valpas1[TAM]="", zero[10]="";
   int resto, num11[TAM], num22[TAM];
   int dif, i;
   char *result;
   result=(char *)malloc(TAM*sizeof(char));

   char num1[TAM];
   char num2[TAM];
   strcpy(num1,num111);
   strcpy(num2,num222);

   dif=0;

   char nume[TAM];
   strcpy(nume, num2);
   strcpy(num2,num1);
   strcpy(num1,nume);

//pra fazer a operacao precisamos ter strings de mesmo tamanho, portanto,
//se, por ex, num2=3 e num1=455, precisaremos transformar num2 em 003
   if(strlen(num2)>strlen(num1)){
//dif eh a diferenca de tamanho entre as strings
      dif=strlen(num2)-strlen(num1);
//loop for q sera executado dif vezes, sendo que, a cada ciclo acrescenta
//um 0 a esquerda do buffer num1
      for(i=0; i<dif; i++){
//esse buffer copnum1 eh usado somente pra evitar problemas com o compilador
//como estamos trabalhando com strings, as vezes nao eh possivel passar um
//retorno de uma funcao que altera uma string X para a propria string X na
//mesma linha
         strcpy(copnum1, num1);
         sprintf(num1, "0%s",copnum1);
      }

   }
//mesma logica do bloco acima, apenas considerando que num1 eh mais extenso
//que num2
   else if(strlen(num1)>strlen(num2)){
      dif=-strlen(num2)+strlen(num1);
      for(i=0; i<dif; i++){
         strcpy(copnum2, num2);
         sprintf(num2, "0%s",copnum2);

      }
   }
//inverte as strings, repare q aqui eu n usei nenhum buffer para a transicao
//isso eh pq no caso da funcao strrev, nao era necessario ja que esse procedimento
//eh realizado dentro dela
   strcpy(num1,strrev(num1));
   strcpy(num2,strrev(num2));
//mais a frente vai dar pra entender essa variavel
   resto=0;
   for(i=0;i<strlen(num1);i++){
//passamos o caractere da posicao i para um inteiro correspondente a ele
//como todo numero em forma de char corresponde ao valor dele + 48 na tabela
//ascii, basta subtrair esses 48 para obter o valor inteiro q o char representa
      num11[i]=num1[i]-48;
      num22[i]=num2[i]-48;
//valpas eh a variavel que armazena o char a ser acresentado na nova string
//sera igual ao valor da coluna i do num11 + o do num22 + o resto que sera explicado
//a frente
      valpas=(num11[i]+num22[i]+resto);
//valpas para a string valpas1
      sprintf(valpas1, "%d", valpas);
//se valpas for menor q 10, teremos apenas um caractere, entao o valor da casa a esquerda
//sera '0'
      if(valpas<10){
         strcpy(zero,"0");
//esse array 'zero' eh so para a concatenaçao do zero a esquerda e copia
         strcat(zero, valpas1);
         strcpy(valpas1, zero);
      }
//o restultado na casa 'i' sera o valor a direita
      result[i]=valpas1[1];
//o resto guardara o valor a esquerda para futuramente ser somado aos valores das casas a
//esquerda
      resto=valpas1[0]-48;

   }
//se o ultimo resto for igual a 1, ele sera acrescentado ao resultado ja que nao existira
//mais valores a serem somados
   if(resto==1)strcat(result,"1");
//reverte a string novamente
   result=strrev(result);
   return(result);

}
//multiplicaçao

//
char *multiplica(char num1[TAM], char num2[TAM]){
   char *result;
   result=(char *) malloc(TAM*sizeof(char));
   int contador;
   char menor[TAM];
   char maior[TAM];

   if(strlen(num1)>=strlen(num2)){
      strcpy(menor,num2);
      strcpy(maior,num1);
   }
   else {
      strcpy(menor,num1);
      strcpy(maior,num2);
   }
   strcpy(menor, strrev(menor));
   int num;
   char guarda[TAM]="";
   char zero[2]="0";
   strcpy(result,"0");

   for(int i=0; i<strlen(menor); i++){

      strcpy(guarda, maior);
      for(int u=0; u<i; u++){
         strcat(guarda, zero);
      }
      num=menor[i]-48;
      for(int j=0; j<num; j++){
         strcpy(result, somadec(result, guarda));
      }
   }
   return(result);
}
//funcao de comparaçao, retorna 2 pra igual a, 1 pra maior que, e 0 pra menor que
//o nome ta incompleto suahsauhsu
int maiorque(char *num1, char *num2){
   if(strlen(num1)>strlen(num2)){
      return 1;
   }
   else if(strlen(num1)<strlen(num2)){
      return 0;
   }
   else if(strlen(num1)==strlen(num2)){
      for(int i=0; i<strlen(num1); i++){
         if(chartoint(num1[i])<chartoint(num2[i])){
            return 0;
            break;
         }
         else if(chartoint(num1[i])>chartoint(num2[i])){
            return 1;
            break;
         }
         else if(chartoint(num1[i])==chartoint(num2[i]) && i==strlen(num1)-1){
            return 2;
         }
         else if(chartoint(num1[i])==chartoint(num2[i])){
            continue;
         }
      }
   }
}
//essa ai ta ai por algum motivo, nem lembro
//maus ai, sao 4:36 da madruga e n vou reler essa merda suahsuahus
char *preenchezeros(char *num, int qtd){
   char *result;
   char troca[strlen(num)];
   result=(char *)malloc(strlen(num)*sizeof(char));
   strcpy(result, num);
   for(int i=0;i<qtd; i++){
      strcpy(troca, result);
      sprintf(result, "0%s",troca);
   }
   return result;
}
//subtraçao
//vai seguindo char por char, transforma cada um em inteiro e subtrai
//o numero menor sempre sera subtraido
//abcd-ABCD
//d-D,c-C ...
//no caso de d-D, se d<D, c eh subtraido em 1 unidade e d somado em 10
//apos isso, eh realizado 10+d-D
//quando chega na ultima casa, a e A, se for igual a 0, o 0 some
char *subtraidec(char *num1, char *num2){
//nao tem negativo, como eh uma funcao para esse programa especifico
//sempre ira subtrair o menor do maior
   char *result;
   char troca[strlen(num1)+strlen(num2)];
   int tamanho;
   char *maior, *menor;
   result=malloc(tamanho*sizeof(char));
   strcpy(result,"0");

   if(maiorque(num1, num2)>0){
      tamanho=strlen(num1);
      strcpy(troca, preenchezeros(num2, strlen(num1)-strlen(num2)));
      maior=(char *)malloc(tamanho*sizeof(char));
      menor=(char *)malloc(tamanho*sizeof(char));
      strcpy(maior, strrev(num1));
      strcpy(menor, strrev(troca));
   }
   else {
      tamanho=strlen(num2);
      strcpy(troca,preenchezeros(num1, strlen(num2)-strlen(num1)));
      maior=(char *)malloc(tamanho*sizeof(char));
      menor=(char *)malloc(tamanho*sizeof(char));
      strcpy(maior, strrev(num2));
      strcpy(menor, strrev(troca));
   }
   int i;
   for(i=0;i<strlen(maior);i++){
      if(maior[i]>=menor[i]){

         result[i]=maior[i]-menor[i]+48;

      }
      else {
         maior[i+1]=maior[i+1]-1;
         result[i]=maior[i]+10-menor[i]+48;

      }
   }
   result[i]='\0';
   bzero(&troca, strlen(troca));
   strcpy(troca, strrev(result));
   strcpy(result, tirazeros(troca));
   return result;
}
//potenciaçao
char* eleva(char* num1, int num2){
   char *result;
   result=(char *)malloc(TAM*sizeof(char));
   strcpy(result, "1");
   for(int i=0; i<num2; i++){
      strcpy(result, multiplica(result, num1));
   }
   return result;
}
//inteiro para string, outra frescura
char* inttostring(int num){
   char *result;
   result=(char *)malloc(2*sizeof(char));
   result[0]=num+48;
   result[1]='\0';
   return result;
}
//divisao
char* divide(char* num1, char* num2){
   char* result;
   result=(char*)malloc(TAM*sizeof(char));
   char copnum[TAM];
   char copcop[TAM];
   strcpy(copnum, num1);
   int i=0, j;
   int qtzeros;
   if(maiorque(copnum, subtraidec(num2, "1"))==2){
      strcpy(result, "1");
   }
   else{
      while(maiorque(copnum, subtraidec(num2, "1"))==1){
         qtzeros=strlen(copnum)-strlen(num2);
         if(maiorque( multiplica( num2, eleva( "10",qtzeros ) ), copnum ) ==1 ){
            qtzeros--;
         }
         for(j=0; maiorque( multiplica( inttostring(j),multiplica( num2, eleva( "10",qtzeros ) ) ), copnum ) % 2 == 0; j++ ){
         }
         j--;
         bzero(&copcop, strlen(copcop));
         strcpy(copcop, somadec(result, multiplica( inttostring(j),eleva( "10",qtzeros ) ) ));

         strcpy(result, copcop);
         bzero(&copcop, strlen(copcop));
         strcpy(copcop, copnum);
         strcpy(copcop, subtraidec(copnum, multiplica( inttostring(j),multiplica( num2, eleva( "10",qtzeros ) ) ) ) );
         bzero(&copnum, strlen(copnum));

         strcpy(copnum, copcop);

      }
   }
   return result;
}
//porra, agr começa a logica do problema
//essa ai vai procurar o trecho procura na string string, a partir da posiçao posistart
//retorna a posiçao
int posi(char *string, char *procura, int posistart){
//recebe uma string a ser procurada a partir de uma posicao
   int i=posistart;
   int j;
//executa o loop enquanto conseguir varrer a string inteira
   while(i<strlen(string)){
      j=0;
//enquanto a sequencia de chars da string for igual a de procura
//executa incrementa o index
      while(string[i+j]==procura[j]){
         j++;
      }
//se chegar ao final da string de procura eh pq ela ja foi encontrada
//entao o loop eh morto
      if(j==strlen(procura)){
         break;
      }
      i++;
   }
//se nao foi encontrado o trecho pesquisado, retorna -1
   if(i==strlen(string)){
      i=-1;
   }
   return i;
}
//essa ai retorna o trecho entre o char start e o char final
//ela eh usada pra identificar o valor passado pelo server

char *recorta(char *string, char *start, char *final){
   char *retorno;
//localiza a posicao do caractere de start(o primeiro que aparecer) e do final
   int posistart=posi(string, start,0)+strlen(start);
   int posifinal=posi(string, final, posistart)-1;
//a funcao posi retorna -1 caso nao encontre o trecho, entao, se esse for o caso
//ela retorna uma string nula
   if(posifinal==-1 || posistart==-1){
      retorno=(char *)malloc(1*sizeof(char));
      retorno="\0";
   }
//caso os 2 trechos existam na string
   else{
      retorno=(char *)malloc((posifinal-posistart+2)*sizeof(char));
      int j=0;
      int i;
//comeca a varrer a string na posicao start e vai ate a posicao final
      for(i=posistart;i<=posifinal;i++){
//passa cada char para a string de retorno
         retorno[j]=string[i];
//incrementa o index da string de retorno
         j++;
      }
//finaliza com o char nulo por precaucao
      retorno[j]='\0';
   }
   return retorno;
}

//fatorial, retornando o numero numa string
char* fatorial(char* num){
   char *result;
   result = (char *)malloc(TAM*sizeof(char));
//comecamos colocando o valor 1 em result, pois, como fatorial de N eh um produtorio de todos
//os numeros inteiros menores ou iguais a N, iremos partir do 1
   strcpy(result, "1");
//a funcao maiorque() retorna 1 caso o primeiro seja maior, 2 caso seja igual e 0 caso seja menor
//dessa forma, se num for igual a 0, um bloco vazio sera executado para que o valor 1 seja retornado
//isso pq, fatorial de 0 eh 1
   if(maiorque(num,"0")==2)
   {}
//caso contrario
   else{
//i sera o nosso contador
      char i[TAM];
//i recebe 1
      strcpy(i, "1");
//o bloco sera executado enquanto i for menor que num+1 sendo i incrementado em 1 unidade ao final
      for(;maiorque(i,somadec(num,"1"))==0;strcpy(i,somadec(i,"1"))){
//multiplica result por i e atualiza o valor de result
         strcpy(result, multiplica(result,i));
      }
   }
//retorna result
      return result;
   }

/*
a logica eh a sgt, primeiro eu converto o numero recebido pra uma sequencia de 0s e 1s
0s representam as casas q n podem ser agrupadas com a da direita, 1 sao as que podem
por ex:
4124320542
0110010000
agora vc vai ter q pegar esse resultado e comprimir os 0s, pois nao ocorrem combinaçoes
entre os 0s, dessa forma, uma sequencia de 000 eh igual a uma seguencia de um unico 0
a resultante entao seria:
011010
agora eh preciso dividir essa resultante em substrings que terminem com 0
0 110 e 10
agora calculamos a quantidade de combinaçoes entre cada uma delas, multiplicamos o final
e o resultado eh a resposta:
0 --> 0 --> 1 comb
110 --> 1 10, 11 0, 1 1 0 --> 3 comb
10 --> 1 0, 10 --> 2 comb
resultado: 1*3*2 --> 6 combinaçoes


*/
//conta quantas combinaçoes podem ser feitas com uma substring de tamanho tam, lembrando que
//ela deve ter passado pelo processo de conversao para 0s e 1s citado acima

char* contacomb(int tam){
   char *valor;
   valor=(char *)malloc(TAM*sizeof(char));
   char troca[TAM];
   strcpy(valor, "0");
   int i;
   i=0;
   int q1;
   int q2;
   while(i<=(tam-tam%2)/2){
      q2=i;
      q1=tam-2*q2;
      strcpy(troca, somadec( valor, divide( fatorial( somadec(inttostring(q1), inttostring(q2) ) ), multiplica( fatorial( inttostring(q1) ), fatorial( inttostring(q2) ) ) ) ) );
      strcpy(valor, troca);
      i=i+1;
   }
   return valor;
}
//compacta a sequencia transformando sequencias de 0s em um unico 0
char *compacta(char *entrada){
   char *saida;
   saida=(char *)malloc(strlen(entrada)*sizeof(char));
   int j=0;
   for(int i=0; i<strlen(entrada); i++){
      if((entrada[i]=='0' && entrada[i-1]=='1')||i==0||entrada[i]=='1'){
         saida[j]=entrada[i];
         j++;
      }
      else continue;
   }
   saida[j]='\0';
   return saida;
}
//pega a entrada, converte para 0s e 1s, compacta e calcula a quantidade de combinaçoes de cada substring
//e multiplica os resultados obtendo a quantidade de combinaçoes total

char* qtd(char *entrada){
   int j,i;
   int count=0;
   char interpreta[strlen(entrada)];
   for(j=0; j<strlen(entrada); j++){
      if((chartoint(entrada[j])>2)||(chartoint(entrada[j])==2 && chartoint(entrada[j+1])>5)||(chartoint(entrada[j])==0)||j==strlen(entrada)-1)interpreta[j]='0';
      else interpreta[j]='1';
   }
   interpreta[j]='\0';
   int tam=strlen(interpreta);
   char substring[TAM][tam];
   int k=0,y,b;
   i=0;
   y=0;
   count=0;
   char transita[strlen(interpreta)];
   strcpy(transita, interpreta);
   bzero(&interpreta, strlen(entrada));
   strcpy(interpreta, compacta(transita));
   k=0;

   while(interpreta[k]!='\0'){
      if(interpreta[k]=='0'){
         substring[y][count]=interpreta[k];
         count++;
         substring[y][count]='\0';
         count=0;

         k++;
         y++;

      }

      substring[y][count]=interpreta[k];
      k++;

      count++;
   }
   substring[y][count]='\0';
   char *a;
   a=(char*)malloc(TAM*sizeof(char));
   char troca[TAM];
   strcpy(a, "1");
   for(b=0; b<=y; b++){
      if(substring[b][0]=='1'){
         strcpy(troca, multiplica(a, contacomb(strlen(substring[b]))));
         strcpy(a, troca);
      }
   }
   return a;

}
//pega entrada do servidor e joga nas funçoes retornando a resposta pro server
void main(){
   int Meusocket, numbytes;
   char buf[TAM];
   struct hostent *he;
   struct sockaddr_in seu_endereco;
   int porta=11718;
   if ((he=gethostbyname("104.236.245.201")) == NULL)
   {
      herror("gethostbyname");
      exit(1);
   }
   if ((Meusocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
      perror("socket");
      exit(1);
   }

   seu_endereco.sin_family = AF_INET;
   seu_endereco.sin_port = htons(porta);
   seu_endereco.sin_addr = *((struct in_addr *)he->h_addr);
   bzero(&(seu_endereco.sin_zero), 8);

   if (connect(Meusocket,(struct sockaddr *)&seu_endereco, sizeof(struct sockaddr)) ==-1)
   {
      perror("connect");
      exit(1);
   }
//lima o buffer buf
   bzero(&buf, strlen(buf));
//recebe dados de tamanho maximo TAM=10000 (sim, exagerei msm) pelo socket
   recv(Meusocket, buf, TAM, 0);
//escreve os dados na tela, nessa parte ele vai escrever a mensagem inicial do server
//explicando o desafio
   printf("%s", buf);
//envia "start\n" para o server indicando que quer começar o jogo
   send(Meusocket, "start\n", 6, 0);
   bzero(&buf, strlen(buf));
//recebe mais uma mensagem
   recv(Meusocket, buf, TAM, 0);
//recebe a mensagem "Iniciando ..."
   printf("%s", buf);
   char msg[TAM];
   char mensagem[TAM];
   char desafio[TAM];
   char resposta[TAM];
//loop infinito
   while(1){
      bzero(&buf, strlen(buf));
//recebe o desafio, ex:
//"[+] Desafio 1 - N: 10
//A resposta é:"
      recv(Meusocket, buf, TAM, 0);
      printf("%s", buf);
//recorta a mensagem recebida pegando somente o trecho entre
//o "N: " e a quebra de linha. ou seja, a sequencia de digitos
//e verifica se esse trecho existe na mensagem
      if(recorta(buf, "N: ","\n")!="\0"){
//caso positivo, copia esse trecho recortado para o buffer "mensagem"
         strcpy(mensagem, recorta(buf, "N: ","\n"));
//calcula as combinaçoes por meio da funçao qtd e passa a saida para o buffer
//resposta
         strcpy(resposta, qtd(mensagem));
//passa o buffer resposta para o buffer "msg", acrescido de um \n (enter)
//sim, eu n precisava ter criado uma nova variavel pra isso
         sprintf(msg, "%s\n", resposta);
//envia a resposta para o server
         send(Meusocket, msg, strlen(msg), 0);
//escreve a resposta no output, so pra vc poder conferir
         printf("%s", msg);
         bzero(&buf, strlen(buf));
//recebe uma mensagem de confimaçao ou erro do server e passa para o buffer "buf", ex:
// [+] Correto!
         recv(Meusocket, buf, TAM, 0);
//se nao receber mensagem, ou seja, se o buffer ficar vazio
//eh sinal que o server parou de responder pq vc errou
//entao ele mata o programa pra evitar sujar demais o terminal
         if(buf[0]==0){
            break;
         }
//escreve a saida na tela
         printf("%s", buf);
      }
//caso a mensagem recebida nao seja a que contem o desafio, ele vai escrever ela na tela
//so pra ver cole que eh, acho q nem chega a entrar ai em condiçoes normail, mas deve ter um sentido
//disso estar ai suahsuhaus
      else {
         printf("%s\n", buf);
      }
   }
//fecha o socket
   close(Meusocket);
}
//eh isso ai, a flag eh HACKAFLAG{3h_M3lh0r_us4r_un1c0d3}, n sei se precisava botar ela em algum

