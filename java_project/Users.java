public class Users {


	/**
	 * @brief Função que trata os dados relativos a users, recebidos do
	 * parsing do XML, e os envia para a hashtable.
	 *
	 * Verifica se aquele artigo existe e se aquela revisão também já existe e,
	 * nesse caso, não modifica nada.
	 * Caso contrário, chama as funções de update da hashtable.
	 *
	 * @param users A hashtable de utilizadores.
	 * @param id A ID do contribuidor da revisão.
	 * @param username O username do contribuidor da revisão.
	 * @param articleId O ID do artigo.
	 * @param revisionId O ID da revisão.
	 * @return A estrutura geral atualizada.
	 */
	public static void onPageUsers(HashMap<User> users, long id, String username, long articleId, long revisionId) {

		User articlePtr = users.get(id);

		if ((articlePtr != null) && (articlePtr.getRevisionsHash().get(revisionId) != null)) {

			return null;
		}

		int userWasFound;

		Hashtable.insertOrUpdateUser(users, id, username, userWasFound);

		return null;
	}


	/**
	 * @brief Função que obtém o nome de um contribuidor através do seu ID.
	 *
	 * Verifica se aquele contributor existe e, caso exista, retorna o seu campo username.
	 *
	 * @see getUser()
	 *
	 * @param qs A estrutura geral do programa.
	 * @param id O ID do contribuidor a pesquisar.
	 * @return O username do contribuidor ou @c NULL.
	 */
	public static String getContributorName(HashMap<User> users, long id) {

		User ourUser = users.get(id);

		if (ourUser != null) {
			return ourUser.getUsername();
		} else {
			return null;
		}
	}


	/**
	 * @brief Função que obtém o top dos 10 melhores contribuidores.
	 *
	 * Cria um iterador da hashtable correspondente aos utilizadores.
	 * Cria e inicializa um array de 10 utilizadores.
	 * Itera pela hashtable e compara o número de contribuições do utilizador a ser iterado
	 * com o top, começando pelo último lugar deste.
	 * Caso seja maior ou igual que o último, continua a fazer comparações com os acima.
	 * Caso o número de contribuições entre dois utilizadores forem iguais, é feita a decisão
	 * pelo ID destes.
	 * Caso entre no top, é feito o reajuste dos lugares abaixo.
	 *
	 * @see getHashtableIterator()
	 * @see getNextFromIterator()
	 *
	 * @param qs A estrutura geral do programa.
	 * @return Um array com os IDs dos 10 melhores contribuidores.
	 */
	long* getTop10Contributors(HashMap<User> users) {

		/* Ideia para iterar
		Map<Integer, Integer> map = new HashMap<Integer, Integer>();
			for (Map.Entry<Integer, Integer> entry : map.entrySet()) {
			    System.out.println("Key = " + entry.getKey() + ", Value = " + entry.getValue());
			}
		*/

		/*
		// Iterating over values only
		for (Integer value : map.values()) {
		    System.out.println("Value = " + value);
		}
		*/

		// Getting top 10 contributors.

		void *iterator = getHashtableIterator(qs->users);

		User[] top10[] = new User[10];

		void *key = NULL;
		struct user *curUser = NULL;

		int index;

		// Iterating through users hash table.
		// Iterar pela hash table de utilizadores.
		while (getNextFromIterator(iterator, &key, &curUser)) {

			index = 9; // Rank onde o utilizador vai ser colocado.

			if (curUser->contributions >= (top10[9])->contributions) {

				// A user's contributions >= top10[9]'s contributions.

				// Diminuir o indíce enquanto o utilizador encaixar num rank superior.
				while (index > 0 && curUser->contributions >= (top10[index - 1])->contributions) {
					index--;
				}

				// This user will be compared to index.

				// Neste ponto sabemos que o score deste user >= top[index].

				if (curUser->contributions == (top10[index])->contributions) {

					// The users contributions are equal.

					// Score deste user == top[index].
					// Comparar usernames alfabeticamente.

					int result = strcmp(curUser->username, (top10[index])->username);

					if (result < 0) {

						// The user alphabetically lost.

						index++; // Colocar este utilizador uma posição abaixo no rank.

						if (index > 9) {

							// "User dropped to 11th place.

							continue; // Utilizador desceu para 11º, skipar.
						}

					} else if (result == 0) {
						fprintf(stderr, "Erro ao comparar contribuicoes dos utilizadores %s e %s\n", curUser->username, (top10[index])->username);
					}

					// The user alphabetically won.
				}

				// Deslizar os outros utilizadores para baixo para termos espaço para o novo utilizador no rank.

				int i;
				for (i = 9; i > index; i--) {

					top10[i] = top10[i - 1];
				}

				// Colocar o utlizador na sua nova posição no rank.
				top10[index] = curUser;
			}
		}

		// Importante - libertar a memória do iterador no final!
		freeIterator(iterator);

		// Obter array de ids e retorná-lo.

		long *top10Ids = malloc(10 * sizeof(long));

		int j;
		for (j = 0; j < 10; j++) {
			top10Ids[j] = (top10[j])->id;
		}

		return top10Ids;
	}

}