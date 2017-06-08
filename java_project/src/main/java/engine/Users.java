package engine;

import java.util.*;
import static java.util.stream.Collectors.toCollection;;

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
    public static void onPageUsers(HashMap<Long, User> users, HashMap<Long, Article> articles, long id, String username, long articleId, long revisionId) {

        Article article = articles.get(articleId);

        if ((article != null) && (article.getRevisionsHash().get(revisionId) != null)) {

            return;
        }

        int userWasFound = -1;

        Hashtable.insertOrUpdateUser(users, id, username, userWasFound);

        return;
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
    public static String getContributorName(HashMap<Long, User> users, long id) {

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
     * FALTA EXPLICAR A FUNCAO
     *
     * @see getHashtableIterator()
     * @see getNextFromIterator()
     *
     * @param qs A estrutura geral do programa.
     * @return Um array com os IDs dos 10 melhores contribuidores.
     */

    public static ArrayList<Long> getTop_10_contributors(HashMap<Long, User> users) {

        ArrayList<User> top10sorted = new ArrayList<User>();

        Comparator<User> comparator = Comparator.comparingLong(User::getUserContributions);
        Comparator<User> reverseComparator = comparator.reversed();

        top10sorted = users.values().stream()
            .sorted(reverseComparator)
            .limit(10)
            .collect(toCollection(ArrayList::new));

        ArrayList<Long> top10 = new ArrayList<Long>();

        for(User u: top10sorted) {
            top10.add(u.getUserId());
        }


        return top10;
    }
}