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
	 * @return A estrutura atualizada.
	 */
	public static void insertOrUpdateUser(HashMap<Long, User> users, long id, String username) {

		// DEBUG - passa direito os argumentos
		/*
		System.out.println("id: " + id);
		System.out.println("username: " + username);
		*/


		//printf("hashtable.c - Received user data\n");

		// Procurar pelo utilizador na hash table
		User userData = users.get(id);
		// System.out.println("1");

		if (userData == null) {

			//printf("Creating new user with id %ld\n", id);

			User newUser = new User();
			newUser.setUserId(id);
			newUser.addUserContribution();
			newUser.setUsername(username);

			users.put(newUser.getUserId(), newUser);
			// DEBUG
			// System.out.println(newUser.toString());

		} else {

			//printf("Updating user with id %ld\n", id);

			// Contar uma contribuição do utilizador
			userData.addUserContribution();
			// System.out.println(userData.getUserContributions());

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
	 * @param revisionParentId O ID do pai da revisão.
	 * @param revisionTimestamp O timestamp da revisão.
	 * @param sizeBytes O tamanho do artigo.
	 * @param nWords O número de palavras do artigo.
	 * @param articleFound Passado para fazer controlo nos contadores de articles.
	 * @param articleUpdated Passado para fazer controlo nos contadores de articles.
	 * @return 0 se foi inserido, 1 se foi atualizado, 2 se é duplicado
	 */

	private static ArrayList<Long> teste;

	public static int insertOrUpdateArticle(HashMap<Long, Article> articles, long id, String title, long revisionId, long revisionParentId, String revisionTimestamp, long sizeBytes, long nWords) {

		if (id == 39) {
			System.out.println("Received article 39");
		}

		//printf("hashtable.c - Received article revision data\n");

		// DEBUG - Estão a entrar bem os elementos
		/*
		System.out.println("Article ID: " + id);
		System.out.println("title: " + title);
		System.out.println("revisionId: " + revisionId);
		System.out.println("revisionParentId: " + revisionParentId);
		System.out.println("revisionTimestamp: " + revisionTimestamp +"\n");
		*/

		// Criar nova revisão (necessária caso o artigo já exista ou não)
		Revision newRevision = new Revision(revisionId, revisionParentId, revisionTimestamp);

		// Verificar se este artigo já existe na hash table

		Article articleData = articles.get(id);

		if (articleData == null) { // Já testei com contaisKey(id) e o resultado não muda, portanto isto está OK

			//printf("Creating new article and adding revision...\n");

			// Criar novo artigo
			Article newArticle = new Article(id, sizeBytes, nWords, title);

			newArticle.addRevision(newRevision.getRevisionId(), newRevision);

			// Adicionar novo artigo à hashtable
			articles.put(newArticle.getArticleId(), newArticle);

			// DEBUG PUT
			//System.out.println("PUT with id: " + newArticle.getArticleId());
			//System.out.println(newArticle.toString());
			// GET
			//System.out.println("GET");
			//System.out.println(articles.get(id).toString());

			// Informar que o artigo foi inserido
			return 0;

		} else {

			System.out.println("Updating article... id: \n" + String.valueOf(id) + " revision id: " + String.valueOf(revisionId));

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

				System.out.println("DUPLICATED");

				// Informar que é duplicado
				return 2;

			} else {

				//printf("Adding revision...\n");
				articleData.addRevision(newRevision.getRevisionId(), newRevision);

				//System.out.println("UPDATED");

				// Informar que foi atualizado
				return 1;
			}
			// DEBUG
			//System.out.println(articleData.toString());
			// GET
			//System.out.println("GET");
			//System.out.println(articles.get(id).toString() + "\n");
		}
	}
}
