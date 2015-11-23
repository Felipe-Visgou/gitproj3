/***************************************************************************
*  $MCI Módulo de implementação: LIS  Lista duplamente encadeada
*
*  Arquivo gerado:              LISTA.c
*  Letras identificadoras:      LIS
*
*  Nome da base de software:    Arcabouço para a automação de testes de programas redigidos em C
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\LISTA.BSW
*
*  Projeto: INF 1301 / 1628 Automatização dos testes de módulos C
*  Gestor:  LES/DI/PUC-Rio
*  Autores: avs
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*     4       avs   01/fev/2006 criar linguagem script simbólica
*     3       avs   08/dez/2004 uniformização dos exemplos
*     2       avs   07/jul/2003 unificação de todos os módulos em um só projeto
*     1       avs   16/abr/2003 início desenvolvimento
*
***************************************************************************/

#include   <stdio.h>
#include   <string.h>
#include   <memory.h>
#include   <malloc.h>
#include   <assert.h>

#ifdef _DEBUG
#include "GENERICO.H"
#include "CONTA.H"
#include "CESPDIN.H"
#include "IdTiposEspacoLista.def"
#endif


#define LISTA_OWN
#include "LISTA.h"
#undef LISTA_OWN

/***********************************************************************
*
*  $TC Tipo de dados: LIS Elemento da lista
*
*
***********************************************************************/

   typedef struct tagElemLista {

	#ifdef _DEBUG
		  struct LIS_tagLista * listaCabeca;
	#endif
         void * pValor ;
               /* Ponteiro para o valor contido no elemento */

         struct tagElemLista * pAnt ;
               /* Ponteiro para o elemento predecessor */

         struct tagElemLista * pProx ;
               /* Ponteiro para o elemento sucessor */

   } tpElemLista ;

/***********************************************************************
*
*  $TC Tipo de dados: LIS Descritor da cabeça de lista
*
*
***********************************************************************/

   typedef struct LIS_tagLista {

         tpElemLista * pOrigemLista ;
               /* Ponteiro para a origem da lista */

         tpElemLista * pFimLista ;
               /* Ponteiro para o final da lista */

         tpElemLista * pElemCorr ;
               /* Ponteiro para o elemento corrente da lista */

         int numElem ;
               /* Número de elementos da lista */

         void ( * ExcluirValor ) ( void * pValor ) ;
               /* Ponteiro para a função de destruição do valor contido em um elemento */

   } LIS_tpLista ;

#ifdef _DEBUG
/***** Declaração dos tipos de dados da lista*/
   typedef enum {
	   LIS_TipoCabeca,
	   LIS_TipoElemento
   } LIS_TipoEspaco;
#endif

/***** Protótipos das funções encapuladas no módulo *****/

   static void LiberarElemento( LIS_tppLista   pLista ,
                                tpElemLista  * pElem   ) ;

   static tpElemLista * CriarElemento( LIS_tppLista pLista ,
                                       void *       pValor  ) ;

   LIS_tpCondRet verificaElemento (void * ppElem, int* f);

   static void LimparCabeca( LIS_tppLista pLista ) ;


/*********************/

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: LIS  &Criar lista
*  ****/

   LIS_tppLista LIS_CriarLista(
             void   ( * ExcluirValor ) ( void * pDado ) )
   {

      LIS_tpLista * pLista = NULL ;

      pLista = ( LIS_tpLista * ) malloc( sizeof( LIS_tpLista )) ;
      if ( pLista == NULL )
      {
         return NULL ;
      } /* if */

      LimparCabeca( pLista ) ;

      pLista->ExcluirValor = ExcluirValor ;

	#ifdef _DEBUG
         CED_DefinirTipoEspaco( pLista , LIS_TipoCabeca ) ;
    #endif

      return pLista ;

   } /* Fim função: LIS  &Criar lista */

/***************************************************************************
*
*  Função: LIS  &Destruir lista
*  ****/

   void LIS_DestruirLista( LIS_tppLista pLista )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      LIS_EsvaziarLista( pLista ) ;

      free( pLista ) ;

   } /* Fim função: LIS  &Destruir lista */

/***************************************************************************
*
*  Função: LIS  &Esvaziar lista
*  ****/

   void LIS_EsvaziarLista( LIS_tppLista pLista )
   {

      tpElemLista * pElem ;
      tpElemLista * pProx ;

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      pElem = pLista->pOrigemLista ;
      while ( pElem != NULL )
      {
         pProx = pElem->pProx ;
         LiberarElemento( pLista , pElem ) ;
         pElem = pProx ;
      } /* while */

      LimparCabeca( pLista ) ;

   } /* Fim função: LIS  &Esvaziar lista */

/***************************************************************************
*
*  Função: LIS  &Inserir elemento antes
*  ****/

   LIS_tpCondRet LIS_InserirElementoAntes( LIS_tppLista pLista ,
                                           void * pValor        )
   {

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      /* Criar elemento a inerir antes */

         pElem = CriarElemento( pLista , pValor ) ;
         if ( pElem == NULL )
         {
            return LIS_CondRetFaltouMemoria ;
         } /* if */

      /* Encadear o elemento antes do elemento corrente */

         if ( pLista->pElemCorr == NULL )
         {
            pLista->pOrigemLista = pElem ;
            pLista->pFimLista = pElem ;
         } else
         {
            if ( pLista->pElemCorr->pAnt != NULL )
            {
               pElem->pAnt  = pLista->pElemCorr->pAnt ;
               pLista->pElemCorr->pAnt->pProx = pElem ;
            } else
            {
               pLista->pOrigemLista = pElem ;
            } /* if */

            pElem->pProx = pLista->pElemCorr ;
            pLista->pElemCorr->pAnt = pElem ;
         } /* if */

         pLista->pElemCorr = pElem ;

         return LIS_CondRetOK ;

   } /* Fim função: LIS  &Inserir elemento antes */

/***************************************************************************
*
*  Função: LIS  &Inserir elemento após
*  ****/

   LIS_tpCondRet LIS_InserirElementoApos( LIS_tppLista pLista ,
                                          void * pValor        )
      
   {

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      /* Criar elemento a inerir após */

         pElem = CriarElemento( pLista , pValor ) ;
         if ( pElem == NULL )
         {
            return LIS_CondRetFaltouMemoria ;
         } /* if */

      /* Encadear o elemento após o elemento */

         if ( pLista->pElemCorr == NULL )
         {
            pLista->pOrigemLista = pElem ;
            pLista->pFimLista = pElem ;
         } else
         {
            if ( pLista->pElemCorr->pProx != NULL )
            {
               pElem->pProx  = pLista->pElemCorr->pProx ;
               pLista->pElemCorr->pProx->pAnt = pElem ;
            } else
            {
               pLista->pFimLista = pElem ;
            } /* if */

            pElem->pAnt = pLista->pElemCorr ;
            pLista->pElemCorr->pProx = pElem ;

         } /* if */
                  
         pLista->pElemCorr = pElem ;
                  
         return LIS_CondRetOK ;

   } /* Fim função: LIS  &Inserir elemento após */

/***************************************************************************
*
*  Função: LIS  &Excluir elemento
*  ****/

   LIS_tpCondRet LIS_ExcluirElemento( LIS_tppLista pLista )
   {

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista  != NULL ) ;
      #endif

      if ( pLista->pElemCorr == NULL )
      {
         return LIS_CondRetListaVazia ;
      } /* if */

      pElem = pLista->pElemCorr ;

      /* Desencadeia à esquerda */

         if ( pElem->pAnt != NULL )
         {
            pElem->pAnt->pProx   = pElem->pProx ;
            pLista->pElemCorr    = pElem->pAnt ;
         } else {
            pLista->pElemCorr    = pElem->pProx ;
            pLista->pOrigemLista = pLista->pElemCorr ;
         } /* if */

      /* Desencadeia à direita */

         if ( pElem->pProx != NULL )
         {
            pElem->pProx->pAnt = pElem->pAnt ;
         } else
         {
            pLista->pFimLista = pElem->pAnt ;
         } /* if */

      LiberarElemento( pLista , pElem ) ;

      return LIS_CondRetOK ;

   } /* Fim função: LIS  &Excluir elemento */

/***************************************************************************
*
*  Função: LIS  &Obter referência para o valor contido no elemento
*  ****/

   void * LIS_ObterValor( LIS_tppLista pLista )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      if ( pLista->pElemCorr == NULL )
      {
        return NULL ;
      } /* if */

      return pLista->pElemCorr->pValor ;

   } /* Fim função: LIS  &Obter referência para o valor contido no elemento */

/***************************************************************************
*
*  Função: LIS  &Ir para o elemento inicial
*  ****/

   void IrInicioLista( LIS_tppLista pLista )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      pLista->pElemCorr = pLista->pOrigemLista ;

   } /* Fim função: LIS  &Ir para o elemento inicial */

/***************************************************************************
*
*  Função: LIS  &Ir para o elemento final
*  ****/

   void IrFinalLista( LIS_tppLista pLista )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      pLista->pElemCorr = pLista->pFimLista ;

   } /* Fim função: LIS  &Ir para o elemento final */

/***************************************************************************
*
*  Função: LIS  &Avançar elemento
*  ****/

   LIS_tpCondRet LIS_AvancarElementoCorrente( LIS_tppLista pLista ,
                                              int numElem )
   {

      int i ;

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      /* Tratar lista vazia */

         if ( pLista->pElemCorr == NULL )
         {

            return LIS_CondRetListaVazia ;

         } /* fim ativa: Tratar lista vazia */

      /* Tratar avançar para frente */

         if ( numElem > 0 )
         {

            pElem = pLista->pElemCorr ;
            for( i = numElem ; i > 0 ; i-- )
            {
               if ( pElem == NULL )
               {
                  break ;
               } /* if */
               pElem    = pElem->pProx ;
            } /* for */

            if ( pElem != NULL )
            {
               pLista->pElemCorr = pElem ;
               return LIS_CondRetOK ;
            } /* if */

            pLista->pElemCorr = pLista->pFimLista ;
            return LIS_CondRetFimLista ;

         } /* fim ativa: Tratar avançar para frente */

      /* Tratar avançar para trás */

         else if ( numElem < 0 )
         {

            pElem = pLista->pElemCorr ;
            for( i = numElem ; i < 0 ; i++ )
            {
               if ( pElem == NULL )
               {
                  break ;
               } /* if */
               pElem    = pElem->pAnt ;
            } /* for */

            if ( pElem != NULL )
            {
               pLista->pElemCorr = pElem ;
               return LIS_CondRetOK ;
            } /* if */

            pLista->pElemCorr = pLista->pOrigemLista ;
            return LIS_CondRetFimLista ;

         } /* fim ativa: Tratar avançar para trás */

      /* Tratar não avançar */

         return LIS_CondRetOK ;

   } /* Fim função: LIS  &Avançar elemento */

/***************************************************************************
*
*  Função: LIS  &Procurar elemento contendo valor
*  ****/

   LIS_tpCondRet LIS_ProcurarValor( LIS_tppLista pLista ,
                                    void * pValor        )
   {

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista  != NULL ) ;
      #endif

      if ( pLista->pElemCorr == NULL )
      {
         return LIS_CondRetListaVazia ;
      } /* if */

      for ( pElem  = pLista->pElemCorr ;
            pElem != NULL ;
            pElem  = pElem->pProx )
      {
         if ( pElem->pValor == pValor )
         {
            pLista->pElemCorr = pElem ;
            return LIS_CondRetOK ;
         } /* if */
      } /* for */

      return LIS_CondRetNaoAchou ;

   } /* Fim função: LIS  &Procurar elemento contendo valor */

/* Obter tamanho da lista */
LIS_tpCondRet LIS_ObterTamanho( LIS_tppLista pLista,int * num)
   {
		*num = pLista->numElem;
		return LIS_CondRetOK ;
   } /* Fim função: LIS  &Obter referência para o tamanho da lista*/

/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: LIS  -Liberar elemento da lista
*
*  $ED Descrição da função
*     Elimina os espaços apontados pelo valor do elemento e o
*     próprio elemento.
*
***********************************************************************/

   void LiberarElemento( LIS_tppLista   pLista ,
                         tpElemLista  * pElem   )
   {

      if ( ( pLista->ExcluirValor != NULL )
        && ( pElem->pValor != NULL        ))
      {
         pLista->ExcluirValor( pElem->pValor ) ;
      } /* if */

      free( pElem ) ;

      pLista->numElem-- ;

   } /* Fim função: LIS  -Liberar elemento da lista */


/***********************************************************************
*
*  $FC Função: LIS  -Criar o elemento
*
***********************************************************************/

   tpElemLista * CriarElemento( LIS_tppLista pLista ,
                                void *       pValor  )
   {

      tpElemLista * pElem ;

      pElem = ( tpElemLista * ) malloc( sizeof( tpElemLista )) ;
      if ( pElem == NULL )
      {
         return NULL ;
      } /* if */

	#ifdef _DEBUG
      CED_DefinirTipoEspaco( pElem , LIS_TipoElemento ) ;
	  pElem->listaCabeca = pLista;
	#endif

      pElem->pValor = pValor ;
      pElem->pAnt   = NULL  ;
      pElem->pProx  = NULL  ;

      pLista->numElem ++ ;

      return pElem ;

   } /* Fim função: LIS  -Criar o elemento */


/***********************************************************************
*
*  $FC Função: LIS  -Limpar a cabeça da lista
*
***********************************************************************/

   void LimparCabeca( LIS_tppLista pLista )
   {

      pLista->pOrigemLista = NULL ;
      pLista->pFimLista = NULL ;
      pLista->pElemCorr = NULL ;
      pLista->numElem   = 0 ;

   } /* Fim função: LIS  -Limpar a cabeça da lista */


#ifdef _DEBUG

   LIS_tpCondRet LIS_DesencadeiaSemFree( LIS_tppLista pLista )
   {

	   tpElemLista * pElem ;

	   assert( pLista  != NULL ) ;

	   if ( pLista->pElemCorr == NULL )
	   {
		   return LIS_CondRetListaVazia ;
	   } /* if */

	   pElem = pLista->pElemCorr ;

	   /* Desencadeia à esquerda */

	   if ( pElem->pAnt != NULL )
	   {
		   pElem->pAnt->pProx   = pElem->pProx ;
		   pLista->pElemCorr    = pElem->pAnt ;
	   } else {
		   pLista->pElemCorr    = pElem->pProx ;
		   pLista->pOrigemLista = pLista->pElemCorr ;
	   } /* if */

	   /* Desencadeia à direita */

	   if ( pElem->pProx != NULL )
	   {
		   pElem->pProx->pAnt = pElem->pAnt ;
	   } else
	   {
		   pLista->pFimLista = pElem->pAnt ;
	   } /* if */

	   pLista->numElem-- ;

	   return LIS_CondRetOK ;

   } /* Fim função: Desencadeia sem liberar o espaço */

#endif

#ifdef _DEBUG
/********** Fim do módulo de implementação: LIS  Lista duplamente encadeada **********/
void DeturpaLista ( void* pLista, LIS_ModosDeturpacao Deturpacao)
{
	LIS_tpLista * Lista = NULL;
	struct tagElemLista* lixo = (struct tagElemLista*)malloc(sizeof(struct tagElemLista));
	lixo->pAnt = NULL;
	lixo->pProx = NULL;
	lixo->pValor = NULL;
	lixo->listaCabeca = Lista;
	if(pLista == NULL)
		return;
	Lista = (LIS_tpLista*)(pLista);
	LIS_AvancarElementoCorrente(Lista, 2);
	switch (Deturpacao) {

		/* Elimina o nó corrente da estrutura */

		case DeturpaEliminaCorr :
		{
			Lista->pElemCorr->pAnt->pProx = lixo;
			Lista->pElemCorr->pProx->pAnt = lixo;
			LiberarElemento(Lista, Lista->pElemCorr);
			break;
		}

		 /* Anula o ponteiro para o próximo elemento da estrutura */

		case DeturpaPtProxNulo :
		{

			Lista->pElemCorr->pProx = NULL;
			break;
			
		}

		/* Anula o ponteiro para o elemento anterior */

		case DeturpaPtAntNulo :
			{
		
			Lista->pElemCorr->pAnt = NULL;
			break;

			}

		 /* Atribui Lixo para o ponteiro do próximo elemento */

		case DeturpaPtProxLixo:
		{
			Lista->pElemCorr->pProx = lixo;
			break;
		}

		 /* Atribui Lixo para o ponteiro para o elemento anterior */

		case DeturpaPtAntLixo :
		{
			Lista->pElemCorr->pAnt = lixo;
			break;
		}

		/* Atribui Nulo ao conteúdo do nó corrente */

		case DeturpaPtConteudoCorrNulo :
		{
			Lista->pElemCorr->pValor = NULL;
			break;
		}

		/* Modifica o tipo de espaço de dados do nó corrente */

		case DeturpaTipoCorr :
		{
			CED_DefinirTipoEspaco( Lista->pElemCorr->pValor , CED_ID_TIPO_VALOR_NULO ) ;
			break;
		}

		/* Elimina o elemento corrente sem usar free() */

		case DeturpaEliminaSemFree :
		{
			LIS_DesencadeiaSemFree(Lista);
			break;
		}

		/* Atribui Nulo ao elemento corrente */

		case DeturpaPtCorrNulo :
		{
			Lista->pElemCorr = NULL;
			break;
		}

		 /* Atribui nulo ao ponteiro de origem da estrutura */

		case DeturpaPtOrigemNulo :
		{
			Lista->pOrigemLista = NULL;
			break;
		}

		default:
			break;
	} /* Fim Switch Deturpacao */
}
#endif

#ifdef _DEBUG
void verificaLista (LIS_tppLista pLista, int* qtd)
{
	int i, num, cont = 0;
	void* valorCorr, *valorAux;
	char* c;
	LIS_tppLista listaAux;
	int qtdFalhas = 0, f;
	struct tagElemLista *elemAux;

	CED_MarcarTodosEspacosInativos(); // passo para verificar vazamento de memoria

	elemAux = pLista->pElemCorr;
	if(elemAux == NULL)// o corrente foi deturpado para nulo (deturpa 9)
	{
		// conta
		// entao foi o deturpa 9
		TST_NotificarFalha("O ponteiro para o elemento corrente é NULL");
		qtdFalhas++;
	}
	else
	{
		//conta
		// entao elemAux != NULL
		valorCorr = pLista->pElemCorr->pValor;
		if(valorCorr == NULL)
		{
			//conta
			TST_NotificarFalha("Ponteiro para o conteudo do no eh NULL");
			qtdFalhas++;
		}
		else
		{
			// conta
			// o elemento é valido, vale checar seus adjacentes e seus tipos de espaço
			// Como saber se é um elemento válido?
			// e se n for...
			// Como saber se foi dado free no elemento?
			// Como saber se foi atribuido lixo ao elemento?
			elemAux = pLista->pElemCorr->pProx;
			// verifica a intergridade do elemento
			verificaElemento(elemAux, &f);
			qtdFalhas+=f;
			if(elemAux == NULL)// entao proximo do corrente foi deturpado
			{
				// conta
				TST_NotificarFalha("Ponteiro para o proximo do corrente eh NULL");
				qtdFalhas++;
			}
			else
			{
				//conta
				elemAux = pLista->pElemCorr->pAnt;
				if(elemAux == NULL) // entao o anterior do corrente foi deturpado
				{
					// conta
					TST_NotificarFalha("Ponteiro para o anterior do corrente eh NULL");
					qtdFalhas++;
				}
				else
				{
					//conta
					// agora vale checar se o prox ou o anterior sao lixo
					elemAux = pLista->pElemCorr->pProx;
					if(elemAux->pValor == NULL) // proximo é lixo
					{
						// conta
						TST_NotificarFalha("Ponteiro para o proximo do corrente eh lixo");
						qtdFalhas++;
					}
					else
					{
						//conta
						elemAux = pLista->pElemCorr->pAnt;
						if(elemAux->pValor == NULL)// anterior é lixo
						{
							// conta
							TST_NotificarFalha("Ponteiro para o anterior do corrente eh lixo");
							qtdFalhas++;
						}
						else
						{
							//conta
							if(pLista->pOrigemLista == NULL)
							{
								// conta
								TST_NotificarFalha("Ponteiro para a origem da lista é NULL");
								qtdFalhas++;
							}
						}
					}
				}
			}
		}
	}
	*qtd = qtdFalhas;
}
#endif

#ifdef _DEBUG

LIS_tpCondRet verificaElemento (void * ppElem, int* f)
{
	struct tagElemLista * pElem;
	int falha = 0;
	pElem = (tpElemLista*)(ppElem);

	// marca elemento ativo
	CED_MarcarEspacoAtivo(pElem);

	// verifica tipo de dado do elemento
	if ( TST_CompararInt( LIS_TipoElemento ,
		CED_ObterTipoEspaco( pElem) ,
		"Tipo do espaço de dados não é elemento da lista." ) != TST_CondRetOK )
	{
		//conta
		falha++;
	}
	// marca o campo valor como ativo
	CED_MarcarEspacoAtivo(pElem->pValor);

	*f = falha;
	return LIS_CondRetOK;
}

#endif
	
/********** Fim do módulo de implementação: LIS  Lista duplamente encadeada **********/

