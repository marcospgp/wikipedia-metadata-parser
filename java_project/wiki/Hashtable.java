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
    }// variáveis de instância - substitua o exemplo abaixo pelo seu próprio
        
}


