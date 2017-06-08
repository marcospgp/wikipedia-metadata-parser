package engine;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.Iterator;
import java.util.ArrayList;
import java.util.List;
import java.util.HashMap;

import javax.xml.stream.XMLEventReader;
import javax.xml.stream.XMLInputFactory;
import javax.xml.stream.XMLStreamConstants;
import javax.xml.stream.XMLStreamException;
import javax.xml.stream.events.Attribute;
import javax.xml.stream.events.Characters;
import javax.xml.stream.events.EndElement;
import javax.xml.stream.events.StartElement;
import javax.xml.stream.events.XMLEvent;

public class Parser {

  public static void parseWikiData(HashMap<Long, Article> articles, HashMap<Long, User> users, int nsnaps, ArrayList<String> snaps_paths) {

    String fileName;
    int i;

    for (i = 0; i < nsnaps; i++) {
      fileName = snaps_paths.get(i);
      System.out.println(fileName);
      parseXML(articles, users, fileName);
    }

    // Testar recolha de dados de artigos
    /*
    for(Article curArticle : articles.values()){
      System.out.println(curArticle.toString()); // tenho de definir toString do articles
    }

    // Testar recolha de dados de users
    /*
    for(User curContributor : users.values()){
      System.out.println(curContributor.toString()); // tenho de definir toString do articles
    }
    */
  }


  private static void parseXML(HashMap<Long, Article> articles, HashMap<Long, User> users, String fileName) {

    String title = null, revisionContributorUsername = null, revisionText = null, revisionTimestamp = null;
    long articleId = -1, revisionId = -1, revisionParentId = -1, revisionContributorId = -1;
    Article curArticle = null;
    User curUser = null;
    Revision curRevision = null;
    boolean hasPageTitle = false, hasPageId = false,
            hasRevisionId = false, hasRevisionParentId = false, hasRevisionTimestamp = false,
            hasContributorUsername = false, hasContributorId = false,
            onContributor = false, onRevision = false;


    System.out.println("Comecei o parse.\n");

    try {
        XMLInputFactory factory = XMLInputFactory.newInstance();
        XMLEventReader eventReader = factory.createXMLEventReader(new FileReader(fileName));

            while(eventReader.hasNext()){
              XMLEvent event = eventReader.nextEvent();
              switch(event.getEventType()){
                  case XMLStreamConstants.START_ELEMENT:
                    StartElement startElement = event.asStartElement();
                    String qName = startElement.getName().getLocalPart();
                    if (qName.equalsIgnoreCase("page")) {
                      curArticle = new Article();
                      curUser = new User();
                      System.out.println("Start Element : page");
                    } else if (qName.equalsIgnoreCase("title")) {
                        hasPageTitle = true;
                    } else if (qName.equalsIgnoreCase("id") && onContributor) {
                        hasContributorId = true;
                    } else if (qName.equalsIgnoreCase("revision")) {
                        onRevision = true;
                        curRevision = new Revision();
                        System.out.println("Start Element : revision");
                    } else if (qName.equalsIgnoreCase("id") && onRevision) {
                        hasRevisionId = true;
                    } else if (qName.equalsIgnoreCase("parentid")) {
                        hasRevisionParentId = true;
                    } else if (qName.equalsIgnoreCase("timestamp")) {
                        hasRevisionTimestamp = true;
                    } else if (qName.equalsIgnoreCase("contributor")) {
                        onContributor = true;
                        System.out.println("Start Element : contributor");
                    } else if (qName.equalsIgnoreCase("username")) {
                        hasContributorUsername = true;
                    } else if (qName.equalsIgnoreCase("id")) {
                        hasPageId = true;
                    }
                    break;
                  case XMLStreamConstants.CHARACTERS:
                    Characters characters = event.asCharacters();
                    if(hasPageTitle){
                        curArticle.setArticleTitle(characters.getData());
                        System.out.println("Title: " + curArticle.getArticleTitle() + "\n");
                        hasPageTitle = false;
                    }
                    if(hasPageId){
                        curArticle.setArticleId(Long.parseLong(characters.getData()));
                        System.out.println("Article id: " + curArticle.getArticleId() + "\n");
                        hasPageId = false;
                    }
                    if(hasRevisionId){
                        curRevision.setRevisionId(Long.parseLong(characters.getData()));
                        System.out.println("Revision id: " + curRevision.getRevisionId() + "\n");
                        hasRevisionId = false;
                        onRevision = false;
                    }
                    if(hasRevisionParentId){
                        curRevision.setRevisionParentId(Long.parseLong(characters.getData()));
                        System.out.println("Revision Parent id: " + curRevision.getRevisionParentId() + "\n");
                        hasRevisionParentId = false;
                    }
                    if(hasRevisionTimestamp){
                        curRevision.setTimestamp(characters.getData());
                        System.out.println("Revision timestamp: " + curRevision.getTimestamp() + "\n");
                        hasRevisionTimestamp = false;
                    }
                    if(hasContributorUsername){
                        curUser.setUsername(characters.getData());
                        System.out.println("User name: " + curUser.getUsername() + "\n");
                        hasContributorUsername = false;
                    }
                    if(hasContributorId){
                        curUser.setUserId(Long.parseLong(characters.getData()));
                        System.out.println("User id: " + curUser.getUserId() + "\n");
                        hasContributorId = false;
                        onContributor = false;
                    }
                    break;
                  case  XMLStreamConstants.END_ELEMENT:
                    EndElement endElement = event.asEndElement();
                    if(endElement.getName().getLocalPart().equalsIgnoreCase("page")){
                        articles.put(curArticle.getArticleId(), curArticle);
                        users.put(curUser.getUserId(), curUser);
                        System.out.println("End Element : page");
                        System.out.println();
                    }
                    break;
              }
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (XMLStreamException e) {
            e.printStackTrace();
    }
  }

}
