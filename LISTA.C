/***************************************************************************
*  $MCI M�dulo de implementa��o: LIS  Lista duplamente encadeada
*
*  Arquivo gerado:              LISTA.c
*  Letras identificadoras:      LIS
*
*  Nome da base de software:    Arcabou�o para a automa��o de testes de programas redigidos em C
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\LISTA.BSW
*
*  Projeto: INF 1301 / 1628 Automatiza��o dos testes de m�dulos C
*  Gestor:  LES/DI/PUC-Rio
*  Autores: avs
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*     4       avs   01/fev/2006 criar linguagem script simb�lica
*     3       avs   08/dez/2004 uniformiza��o dos exemplos
*     2       avs   07/jul/2003 unifica��o de todos os m�dulos em um s� projeto
*     1       avs   16/abr/2003 in�cio desenvolvimento
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
*  $TC Tipo de dados: LIS Descritor da cabe�a de lista
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
               /* N�mero de elementos da lista */

         void ( * ExcluirValor ) ( void * pValor ) ;
               /* Ponteiro para a fun��o de destrui��o do valor contido em um elemento */

   } LIS_tpLista ;

#ifdef _DEBUG
/***** Declara��o dos tipos de dados da lista*/
   typedef enum {
	   LIS_TipoCabeca = 1,
	   LIS_TipoElemento = 2
   } LIS_TipoEspaco;
#endif

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

   static void LiberarElemento( LIS_tppLista   pLista ,
                                tpElemLista  * pElem   ) ;

   static tpElemLista * CriarElemento( LIS_tppLista pLista ,
                                       void *       pValor  ) ;
#ifdef _DEBUG
   LIS_tpCondRet verificaElemento (void * ppElem, int* f);

   int VerificaVazamentoMem(LIS_tppLista pLista);
#endif

   static void LimparCabeca( LIS_tppLista pLista ) ;

   int PercorreLista(LIS_tppLista pLista);
/*********************/
   	struct tagElemLista* lixo = NULL;
/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Fun��o: LIS  &Criar lista
*  ****/

   LIS_tppLista LIS_CriarLista(
             void   ( * ExcluirValor ) ( void * pDado ) )
   {

      LIS_tpLista * pLista = NULL ;

	  #ifdef _DEBUG
	  CNT_CONTAR( "CriarLista" ) ;
		#endif

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

   } /* Fim fun��o: LIS  &Criar lista */

/***************************************************************************
*
*  Fun��o: LIS  &Destruir lista
*  ****/

   void LIS_DestruirLista( LIS_tppLista pLista )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif
	 #ifdef _DEBUG
	  CNT_CONTAR( "DestruirLista" ) ;
	#endif

      LIS_EsvaziarLista( pLista ) ;

      free( pLista ) ;

   } /* Fim fun��o: LIS  &Destruir lista */

/***************************************************************************
*
*  Fun��o: LIS  &Esvaziar lista
*  ****/

   void LIS_EsvaziarLista( LIS_tppLista pLista )
   {

      tpElemLista * pElem ;
      tpElemLista * pProx ;

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif
		#ifdef _DEBUG
	 CNT_CONTAR( "EsvaziarLista" ) ;
		#endif

      pElem = pLista->pOrigemLista ;
      while ( pElem != NULL )
      {
		#ifdef _DEBUG
		 CNT_CONTAR( "EsvaziarListaWhile" ) ;
		#endif
         pProx = pElem->pProx ;
         LiberarElemento( pLista , pElem ) ;
         pElem = pProx ;
      } /* while */

      LimparCabeca( pLista ) ;

   } /* Fim fun��o: LIS  &Esvaziar lista */

/***************************************************************************
*
*  Fun��o: LIS  &Inserir elemento antes
*  ****/

   LIS_tpCondRet LIS_InserirElementoAntes( LIS_tppLista pLista ,
	   void * pValor        )
   {

	   tpElemLista * pElem ;

#ifdef _DEBUG
	   assert( pLista != NULL ) ;
#endif

#ifdef _DEBUG
	   CNT_CONTAR( "InserirElementoAntes" ) ;
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

#ifdef _DEBUG
		   CNT_CONTAR( "InserirElementoAntesif1" ) ;
#endif
	   } 
	   else
	   {
#ifdef _DEBUG
		   CNT_CONTAR( "InserirElementoAnteselse0" ) ;
#endif
		   if ( pLista->pElemCorr->pAnt != NULL )
		   {
			   pElem->pAnt  = pLista->pElemCorr->pAnt ;
			   pLista->pElemCorr->pAnt->pProx = pElem ;

#ifdef _DEBUG
			   CNT_CONTAR( "InserirElementoAntesif2" ) ;
#endif

		   } else
		   {
#ifdef _DEBUG
			   CNT_CONTAR( "InserirElementoAnteselse1" ) ;
#endif
			   pLista->pOrigemLista = pElem ;
		   } /* if */

		   pElem->pProx = pLista->pElemCorr ;
		   pLista->pElemCorr->pAnt = pElem ;
	   } /* if */

	   pLista->pElemCorr = pElem ;

	   return LIS_CondRetOK ;

   } /* Fim fun��o: LIS  &Inserir elemento antes */

/***************************************************************************
*
*  Fun��o: LIS  &Inserir elemento ap�s
*  ****/

   LIS_tpCondRet LIS_InserirElementoApos( LIS_tppLista pLista ,
                                          void * pValor        )
      
   {

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

		 	#ifdef _DEBUG
			CNT_CONTAR( "InserirElementoApos" ) ;
			 #endif


      /* Criar elemento a inerir ap�s */

         pElem = CriarElemento( pLista , pValor ) ;
         if ( pElem == NULL )
         {
            return LIS_CondRetFaltouMemoria ;
         } /* if */

      /* Encadear o elemento ap�s o elemento */

         if ( pLista->pElemCorr == NULL )
         {
            pLista->pOrigemLista = pElem ;
            pLista->pFimLista = pElem ;
					 	#ifdef _DEBUG
			CNT_CONTAR( "InserirElementoAposif0" ) ;
			 #endif
         } else
         {
			 #ifdef _DEBUG
			CNT_CONTAR( "InserirElementoAposelse0" ) ;
			 #endif
            if ( pLista->pElemCorr->pProx != NULL )
            {
			#ifdef _DEBUG
			CNT_CONTAR( "InserirElementoAposif1" ) ;
			 #endif
               pElem->pProx  = pLista->pElemCorr->pProx ;
               pLista->pElemCorr->pProx->pAnt = pElem ;
            } else
            {
			#ifdef _DEBUG
			CNT_CONTAR( "InserirElementoAposelse1" ) ;
			 #endif
               pLista->pFimLista = pElem ;
            } /* if */

            pElem->pAnt = pLista->pElemCorr ;
            pLista->pElemCorr->pProx = pElem ;

         } /* if */
                  
         pLista->pElemCorr = pElem ;
                  
         return LIS_CondRetOK ;

   } /* Fim fun��o: LIS  &Inserir elemento ap�s */

/***************************************************************************
*
*  Fun��o: LIS  &Excluir elemento
*  ****/

   LIS_tpCondRet LIS_ExcluirElemento( LIS_tppLista pLista )
   {

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista  != NULL ) ;
      #endif
	  			#ifdef _DEBUG
			CNT_CONTAR( "ExcluirElemento" ) ;
			 #endif
      if ( pLista->pElemCorr == NULL )
      {
         return LIS_CondRetListaVazia ;
      } /* if */

      pElem = pLista->pElemCorr ;

      /* Desencadeia � esquerda */

         if ( pElem->pAnt != NULL )
         {
            pElem->pAnt->pProx   = pElem->pProx ;
            pLista->pElemCorr    = pElem->pAnt ;
			#ifdef _DEBUG
			CNT_CONTAR( "ExcluirElementoif0" ) ;
			 #endif
         } else {
            pLista->pElemCorr    = pElem->pProx ;
            pLista->pOrigemLista = pLista->pElemCorr ;
			#ifdef _DEBUG
			CNT_CONTAR( "ExcluirElementoelse0" ) ;
			 #endif
         } /* if */

      /* Desencadeia � direita */

         if ( pElem->pProx != NULL )
         {
            pElem->pProx->pAnt = pElem->pAnt ;
			#ifdef _DEBUG
			CNT_CONTAR( "ExcluirElementoif1" ) ;
			 #endif
         } else
         {
            pLista->pFimLista = pElem->pAnt ;
			#ifdef _DEBUG
			CNT_CONTAR( "ExcluirElementoelse1" ) ;
			 #endif
         } /* if */

      LiberarElemento( pLista , pElem ) ;

      return LIS_CondRetOK ;

   } /* Fim fun��o: LIS  &Excluir elemento */

/***************************************************************************
*
*  Fun��o: LIS  &Obter refer�ncia para o valor contido no elemento
*  ****/

   void * LIS_ObterValor( LIS_tppLista pLista )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

	  #ifdef _DEBUG
			CNT_CONTAR( "ObterValor" ) ;
	  #endif

      if ( pLista->pElemCorr == NULL )
      {
        return NULL ;
      } /* if */

      return pLista->pElemCorr->pValor ;

   } /* Fim fun��o: LIS  &Obter refer�ncia para o valor contido no elemento */

/***************************************************************************
*
*  Fun��o: LIS  &Ir para o elemento inicial
*  ****/

   void IrInicioLista( LIS_tppLista pLista )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

	  #ifdef _DEBUG
			CNT_CONTAR( "IrInicioLista" ) ;
	  #endif

      pLista->pElemCorr = pLista->pOrigemLista ;

   } /* Fim fun��o: LIS  &Ir para o elemento inicial */

/***************************************************************************
*
*  Fun��o: LIS  &Ir para o elemento final
*  ****/

   void IrFinalLista( LIS_tppLista pLista )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif
	  #ifdef _DEBUG
			CNT_CONTAR( "IrFinalLista" ) ;
	  #endif

      pLista->pElemCorr = pLista->pFimLista ;

   } /* Fim fun��o: LIS  &Ir para o elemento final */

/***************************************************************************
*
*  Fun��o: LIS  &Avan�ar elemento
*  ****/

   LIS_tpCondRet LIS_AvancarElementoCorrente( LIS_tppLista pLista ,
                                              int numElem )
   {

      int i ;

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

	 #ifdef _DEBUG
			CNT_CONTAR( "AvancarElementoCorrente" ) ;
	 #endif	

      /* Tratar lista vazia */

         if ( pLista->pElemCorr == NULL )
         {

            return LIS_CondRetListaVazia ;

         } /* fim ativa: Tratar lista vazia */

      /* Tratar avan�ar para frente */

         if ( numElem > 0 )
         {
	 #ifdef _DEBUG
			CNT_CONTAR( "AvancarElementoCorrenteif0" ) ;
	 #endif	
            pElem = pLista->pElemCorr ;
            for( i = numElem ; i > 0 ; i-- )
            {
	 #ifdef _DEBUG
			CNT_CONTAR( "AvancarElementoCorrentefor0" ) ;
	 #endif	
               if ( pElem == NULL )
               {
	 #ifdef _DEBUG
			CNT_CONTAR( "AvancarElementoCorrenteif1" ) ;
	 #endif	
                  break ;
               } /* if */
               pElem    = pElem->pProx ;
            } /* for */

            if ( pElem != NULL )
            {
	 #ifdef _DEBUG
			CNT_CONTAR( "AvancarElementoCorrenteif2" ) ;
	 #endif	
               pLista->pElemCorr = pElem ;
               return LIS_CondRetOK ;
            } /* if */

            pLista->pElemCorr = pLista->pFimLista ;
            return LIS_CondRetFimLista ;

         } /* fim ativa: Tratar avan�ar para frente */

      /* Tratar avan�ar para tr�s */

         else if ( numElem < 0 )
         {
	 #ifdef _DEBUG
			CNT_CONTAR( "AvancarElementoCorrenteielse0" ) ;
	 #endif	
            pElem = pLista->pElemCorr ;
            for( i = numElem ; i < 0 ; i++ )
            {
	 #ifdef _DEBUG
			CNT_CONTAR( "AvancarElementoCorrentefor1" ) ;
	 #endif	
               if ( pElem == NULL )
               {
	 #ifdef _DEBUG
			CNT_CONTAR( "AvancarElementoCorrenteif4" ) ;
	 #endif	
                  break ;
               } /* if */
               pElem    = pElem->pAnt ;
            } /* for */

            if ( pElem != NULL )
            {
	 #ifdef _DEBUG
			CNT_CONTAR( "AvancarElementoCorrenteif5" ) ;
	 #endif	
               pLista->pElemCorr = pElem ;
               return LIS_CondRetOK ;
            } /* if */

            pLista->pElemCorr = pLista->pOrigemLista ;
            return LIS_CondRetFimLista ;

         } /* fim ativa: Tratar avan�ar para tr�s */

      /* Tratar n�o avan�ar */

         return LIS_CondRetOK ;

   } /* Fim fun��o: LIS  &Avan�ar elemento */

/***************************************************************************
*
*  Fun��o: LIS  &Procurar elemento contendo valor
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

   } /* Fim fun��o: LIS  &Procurar elemento contendo valor */

/* Obter tamanho da lista */
LIS_tpCondRet LIS_ObterTamanho( LIS_tppLista pLista,int * num)
   {
	   	#ifdef _DEBUG
			CNT_CONTAR( "ObterTamanho" ) ;
		 #endif
		*num = pLista->numElem;
		return LIS_CondRetOK ;
   } /* Fim fun��o: LIS  &Obter refer�ncia para o tamanho da lista*/

/*****  C�digo das fun��es encapsuladas no m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: LIS  -Liberar elemento da lista
*
*  $ED Descri��o da fun��o
*     Elimina os espa�os apontados pelo valor do elemento e o
*     pr�prio elemento.
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

   } /* Fim fun��o: LIS  -Liberar elemento da lista */


/***********************************************************************
*
*  $FC Fun��o: LIS  -Criar o elemento
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

   } /* Fim fun��o: LIS  -Criar o elemento */


/***********************************************************************
*
*  $FC Fun��o: LIS  -Limpar a cabe�a da lista
*
***********************************************************************/

   void LimparCabeca( LIS_tppLista pLista )
   {

      pLista->pOrigemLista = NULL ;
      pLista->pFimLista = NULL ;
      pLista->pElemCorr = NULL ;
      pLista->numElem   = 0 ;

   } /* Fim fun��o: LIS  -Limpar a cabe�a da lista */


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

	   /* Desencadeia � esquerda */

	   if ( pElem->pAnt != NULL )
	   {
		   pElem->pAnt->pProx   = pElem->pProx ;
		   pLista->pElemCorr    = pElem->pAnt ;
	   } else {
		   pLista->pElemCorr    = pElem->pProx ;
		   pLista->pOrigemLista = pLista->pElemCorr ;
	   } /* if */

	   /* Desencadeia � direita */

	   if ( pElem->pProx != NULL )
	   {
		   pElem->pProx->pAnt = pElem->pAnt ;
	   } else
	   {
		   pLista->pFimLista = pElem->pAnt ;
	   } /* if */

	   pLista->numElem-- ;

	   return LIS_CondRetOK ;

   } /* Fim fun��o: Desencadeia sem liberar o espa�o */

#endif

#ifdef _DEBUG
/********** Fim do m�dulo de implementa��o: LIS  Lista duplamente encadeada **********/
void DeturpaLista ( LIS_tppLista pLista, LIS_ModosDeturpacao Deturpacao)
{
	LIS_tpLista * Lista = NULL;
	if(pLista == NULL)
		return;
	Lista = (LIS_tpLista*)(pLista);
	switch (Deturpacao) {

		/* Elimina o n� corrente da estrutura */

	case DeturpaEliminaCorr :
		{
			lixo = ( tpElemLista * ) malloc( sizeof( tpElemLista )) ;
			lixo->pAnt = NULL;
			lixo->pProx = NULL;
			lixo->pValor = NULL;
			lixo->listaCabeca = Lista;
			Lista->pElemCorr->pAnt->pProx = lixo;
			Lista->pElemCorr->pProx->pAnt = lixo;
			LiberarElemento(Lista, Lista->pElemCorr);
			break;
		}

		/* Anula o ponteiro para o pr�ximo elemento da estrutura */

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

		/* Atribui Lixo para o ponteiro do pr�ximo elemento */

	case DeturpaPtProxLixo:
		{
			lixo = ( tpElemLista * ) malloc( sizeof( tpElemLista )) ;
			lixo->pAnt = NULL;
			lixo->pProx = NULL;
			lixo->pValor = NULL;
			lixo->listaCabeca = Lista;
			Lista->pElemCorr->pProx = lixo;
			break;
		}

		/* Atribui Lixo para o ponteiro para o elemento anterior */

	case DeturpaPtAntLixo :
		{
			lixo = ( tpElemLista * ) malloc( sizeof( tpElemLista )) ;
			lixo->pAnt = NULL;
			lixo->pProx = NULL;
			lixo->pValor = NULL;
			lixo->listaCabeca = Lista;
			Lista->pElemCorr->pAnt = lixo;
			break;
		}

		/* Atribui Nulo ao conte�do do n� corrente */

	case DeturpaPtConteudoCorrNulo :
		{
			Lista->pElemCorr->pValor = NULL;
			break;
		}

		/* Modifica o tipo de espa�o de dados do n� corrente */

		case DeturpaTipoCorr :
		{
			CED_DefinirTipoEspaco( Lista->pElemCorr , CED_ID_TIPO_VALOR_NULO ) ;
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

			/* Altera o tipo de espa�o da cabe�a de lista*/
		case DeturpaTipoCabeca :
			{
				CED_DefinirTipoEspaco(Lista , CED_ID_TIPO_VALOR_NULO ) ;
				break;
			}
		default:
			break;
	} /* Fim Switch Deturpacao */
}
#endif

#ifdef _DEBUG
// S� n � poss�vel testar a deturpa 1 e 8
void verificaLista (LIS_tppLista pLista, int* qtd)
{
	int i, num, cont = 0, tamObtido, tipoObtido;
	void* valorCorr, *valorAux;
	char* c;
	LIS_tppLista listaAux;
	int qtdFalhas = 0, f;
	struct tagElemLista *elemAux;
	*qtd = 0;

	CED_MarcarTodosEspacosInativos(); // passo para verificar vazamento de memoria
	CED_MarcarEspacoAtivo(lixo);
	elemAux = pLista->pElemCorr;
	if(elemAux == NULL)// o corrente foi deturpado para nulo (deturpa 9)
	{
		CNT_CONTAR( "ElemCorrNulo" ) ;
		// entao foi o deturpa 9
		TST_NotificarFalha("O ponteiro para o elemento corrente � NULL");
		qtdFalhas++;
	}
	else
	{
		CNT_CONTAR( "NaoElemCorrNulo") ;
		// entao elemAux != NULL
		valorCorr = pLista->pElemCorr->pValor;
		if(valorCorr == NULL)
		{
			CNT_CONTAR( "ElemCorrValorNulo" ) ;
			TST_NotificarFalha("Ponteiro para o conteudo do no eh NULL");
			qtdFalhas++;
		}
		else
		{
			CNT_CONTAR( "NaoElemCorrValorNulo" ) ;
			elemAux = pLista->pElemCorr;
			tamObtido = PercorreLista(pLista);
			if(tamObtido != pLista->numElem)
			{
				CNT_CONTAR( "TamanhoLista" ) ;
				TST_NotificarFalha("Ponteiro para o corrente foi liberado");
				qtdFalhas++;
			}
			else
			{
				CNT_CONTAR( "NaoTamanhoLista" ) ;
				pLista->pElemCorr = elemAux;

				verificaElemento(elemAux, &f);

				elemAux = pLista->pElemCorr->pProx;
				// verifica a intergridade do elemento
				qtdFalhas+=f;
				if(elemAux == NULL)// entao proximo do corrente foi deturpado
				{
					CNT_CONTAR( "ElemCorrPproxNulo" ) ;
					TST_NotificarFalha("Ponteiro para o proximo do corrente eh NULL");
					qtdFalhas++;
				}
				else
				{
					CNT_CONTAR( "NaoElemCorrPproxNulo" ) ;
					elemAux = pLista->pElemCorr->pAnt;
					if(elemAux == NULL) // entao o anterior do corrente foi deturpado
					{
						CNT_CONTAR( "ElemCorrPantNulo" ) ;
						TST_NotificarFalha("Ponteiro para o anterior do corrente eh NULL");
						qtdFalhas++;
					}
					else
					{
						CNT_CONTAR( "NaoElemCorrPantNulo" ) ;
						// agora vale checar se o prox ou o anterior sao lixo
						elemAux = pLista->pElemCorr->pProx;
						if(elemAux->pValor == NULL) // proximo � lixo
						{
 							CNT_CONTAR( "ElemCorrPproxPvalorNulo" ) ;
							TST_NotificarFalha("Ponteiro para o proximo do corrente eh lixo");
							qtdFalhas++;
						}
						else
						{
 							CNT_CONTAR( "NaoElemCorrPproxPvalorNulo" ) ;
							elemAux = pLista->pElemCorr->pAnt;
							if(elemAux->pValor == NULL)// anterior � lixo
							{
								CNT_CONTAR( "ElemCorrPantPvalorNulo" ) ;
								TST_NotificarFalha("Ponteiro para o anterior do corrente eh lixo");
								qtdFalhas++;
							}
							else
							{
 								CNT_CONTAR( "NaoElemCorrPantPvalorNulo" ) ;
								if(pLista->pOrigemLista == NULL)
								{
 									CNT_CONTAR( "OrigemListaNulo" ) ;
									TST_NotificarFalha("Ponteiro para a origem da lista � NULL");
									qtdFalhas++;
								}
								else
								{
 									CNT_CONTAR( "NaoOrigemListaNulo" ) ;
									tipoObtido = CED_ObterTipoEspaco( pLista->pElemCorr );
									if(tipoObtido !=  LIS_TipoElemento)
									{
										CNT_CONTAR( "TipoDaLista" ) ;
										TST_NotificarFalha("Tipo de espaco deturpado, nao eh elemento");
										qtdFalhas++;
									}
									else
									{
										CNT_CONTAR( "NaoTipoDaLista" ) ;
										tipoObtido = CED_ObterTipoEspaco(pLista);
										if(tipoObtido != LIS_TipoCabeca)
										{
												CNT_CONTAR( "TipoCabeca" ) ;
												TST_NotificarFalha("Houve vazamento de memoria");
												qtdFalhas++;
										}
										else
										{
											CNT_CONTAR( "NaoTipoCabeca" ) ;
											qtdFalhas += VerificaVazamentoMem(pLista);
											if(qtdFalhas > 0)
											{
												CNT_CONTAR( "VazamentoDaLista" ) ;
												TST_NotificarFalha("Houve vazamento de memoria");
											}
											else 
												CNT_CONTAR( "NaoVazamentoDaLista" ) ;
										}
									}
								}
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

	CNT_CONTAR("verificaElem");

	// marca elemento ativo
	CED_MarcarEspacoAtivo(pElem);

	// verifica tipo de dado do elemento
	if ( TST_CompararInt( LIS_TipoElemento ,
		CED_ObterTipoEspaco( pElem) ,
		"Tipo do espa�o de dados n�o � elemento da lista." ) != TST_CondRetOK )
	{
		
		CNT_CONTAR("VerificaElemif0");
		falha++;
	}
	else CNT_CONTAR("NaoVerificaElemTipoif0");
	// marca o campo valor como ativo
	CED_MarcarEspacoAtivo(pElem->pValor);

	*f = falha;
	return LIS_CondRetOK;
}

int PercorreLista(LIS_tppLista pLista)
{
	int cont1 = 0, cont2 = 0;
	pLista->pElemCorr = pLista->pOrigemLista;
	while(pLista->pElemCorr != NULL)
	{
		cont1++;
		pLista->pElemCorr = pLista->pElemCorr->pProx;
	}
	// tr�s para a frente
	pLista->pElemCorr = pLista->pFimLista;
	while(pLista->pElemCorr != NULL)
	{
		cont2++;
		pLista->pElemCorr = pLista->pElemCorr->pAnt;
	}
	if(cont1 == pLista->numElem || cont2 == pLista->numElem)
		return pLista->numElem;
	else
		return cont1;
}
int VerificaVazamentoMem(LIS_tppLista pLista)
{
	/*
	void CED_InicializarIteradorEspacos( ) ;
	int CED_AvancarProximoEspaco( ) ;
	void * CED_ObterPonteiroEspacoCorrente( ) ;
	int CED_EhEspacoAtivo( void * Ponteiro ) ;
	void CED_ExibirTodosEspacos( CED_tpModoExibir Regra ) ;
	*/
	void* aux;
	int i, contaInativos = 0, f;
	struct tagElemLista * elemAux;
	void CED_InicializarIteradorEspacos( ) ;
	elemAux = pLista->pOrigemLista;
	// Percorre a estrutura marcando os espa�os ativos
	CED_MarcarEspacoAtivo(pLista);
	while(elemAux != NULL)
	{
		verificaElemento(elemAux, &f);
		elemAux = elemAux->pProx;
	}
	// percorre a lista de espa�os alocados verficando se existe algum espa�o inativo, se tiver, entao houve vazamento
	CED_InicializarIteradorEspacos( ) ;
	do
	{
		aux = CED_ObterPonteiroEspacoCorrente( ) ;
		if(!CED_EhEspacoAtivo( aux ))
			contaInativos++;
	}while(CED_AvancarProximoEspaco( ));

	// exibe todos os espa�os inativos se houver
	if(contaInativos > 0)
	{
		CED_ExibirTodosEspacos( CED_ExibirInativos ) ;
		f = 1;
	}
	CED_TerminarIteradorEspacos();
	return f;
}
#endif
	
/********** Fim do m�dulo de implementa��o: LIS  Lista duplamente encadeada **********/

