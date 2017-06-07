import java.util.*;

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

}