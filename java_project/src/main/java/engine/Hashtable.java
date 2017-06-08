package engine;

import java.util.*;

/**
 * Escreva a descrição da classe Hashtable aqui.
 *
 * @author (seu nome)
 * @version (número de versão ou data)
 */
public class Hashtable {

    /**
     * @brief Insere ou atualiza um utilizador na hashtable de utilizadores.
     *
     * Procurar pelo utilizador na hash table e, caso o utilizador não foi encontrado,
     * criar e inserir novo utilizador.
     * Caso tenha sido encontrado, apenas aumentar uma contribuição ao utilizador.
     *
     * @param qs A estrutura geral do programa.
     * @param id A ID do contribuidor.
     * @param username O username do contribuidor.
     * @param userWasFound Passado para fazer controlo.
     * @return A estrutura atualizada.
     */
    public static void insertOrUpdateUser(HashMap<Long, User> users, long id, String username, int userWasFound) {

    	//printf("hashtable.c - Received user data\n");

    	// Procurar pelo utilizador na hash table
    	User userData = users.get(id);

    	if (userData == null) {

    		//printf("Creating new user with id %ld\n", id);

    		// Informar que o utilizador não foi encontrado
    		userWasFound = 0;

    		User newUser = new User();
    		newUser.setUserId(id);
    		newUser.addUserContribution();
    		newUser.setUsername(username);

    	} else {

    		//printf("Updating user with id %ld\n", id);

    		// Informar que o utilizador foi encontrado
    		userWasFound = 1;

    		// Contar uma contribuição do utilizador
    		userData.addUserContribution();

    		// Nota - não é preciso inserir de novo na hashtable porque o que
    		// está lá é o endereço para a estrutura que alteramos
    	}

    	return;
    }

        /**
     * @brief Insere ou atualiza um artigo na hashtable de utilizadores.
     *
     * Copiar strings porque o parser pode apagá-las e a hash table só guarda um pointer.
     * Copiar longs usadas como key também, porque keys são usadas a partir dos seus pointers.
     * Criar nova revisão (necessária caso o artigo já exista ou não).
     * Informar se o artigo foi encontrado ou não através de articleFound e articleUpdated.
     * Criar ou atualizar o artigo dependendo se este foi encontrado ou não.
     *
     * @param qs A estrutura geral do programa.
     * @param id A ID do artigo.
     * @param title O título do artigo.
     * @param revisionId O ID da revisão.
     * @param revisionTimestamp O timestamp da revisão.
     * @param sizeBytes O tamanho do artigo.
     * @param nWords O número de palavras do artigo.
     * @param articleFound Passado para fazer controlo nos contadores de articles.
     * @param articleUpdated Passado para fazer controlo nos contadores de articles.
     * @return A estrutura atualizada.
     */
    public static void insertOrUpdateArticle(HashMap<Long, Article> articles, long id, String title, long revisionId, long revisionParentId, String revisionTimestamp, long sizeBytes, long nWords, int articleFound, int articleUpdated) {

    	//printf("hashtable.c - Received article revision data\n");

    	// Copiar strings porque o parser pode apagá-las
    	// E a hash table só guarda um pointer
    	// Copiar longs usadas como key também, porque keys
    	// são usadas a partir dos seus pointers


    	// Criar nova revisão (necessária caso o artigo já exista ou não)
    	Revision newRevision = new Revision();

    	newRevision.setRevisionId(revisionId);
    	newRevision.setTimestamp(revisionTimestamp);
    	newRevision.setRevisionParentId(revisionParentId);

    	// Verificar se este artigo já existe na hash table

    	Article articleData = articles.get(id);

    	if (articleData == null) {

    		//printf("Creating new article and adding revision...\n");

    		// Informar que o artigo não foi encontrado
    		articleFound = 0;
    		articleUpdated = 1;


    		// Criar novo artigo

    		Article newArticle = new Article();

    		newArticle.setArticleId(id);
    		newArticle.setArticleSize(sizeBytes);
    		newArticle.setArticleNWords(nWords);
    		newArticle.setArticleTitle(title);
    		newArticle.getRevisionsHash().put(revisionId, newRevision);

    		// Adicionar novo artigo à hashtable
    		articles.put(id, newArticle);

    	} else {

    		//printf("Updating article...\n");

    		// Informar que o artigo foi encontrado
    		articleFound = 1;

    		// Atualizar artigo já existente
    		// Apenas atualizar tamanho e nº de palavras do artigo se for maior que o anterior

    		long currentSizeBytes = articleData.getArticleSize();

    		long currentNWords = articleData.getArticleNWords();

    		if (sizeBytes > currentSizeBytes) {
    			articleData.setArticleSize(sizeBytes);
    		}

    		if (nWords > currentNWords) {
    			articleData.setArticleNWords(nWords);
    		}

    		// Atualizar título

    		articleData.setArticleTitle(title);

    		// Adicionar revisão

    		if (articleData.getRevisionsHash().get(revisionId) != null) {

    			//printf("Duplicated revision. Skipping...\n");
    			articleUpdated = 0;

    		} else {

    			//printf("Adding revision...\n");
    			articleData.getRevisionsHash().put(revisionId, newRevision);
    			articleUpdated = 1;
    		}
    	}

    	return;
    }

}


