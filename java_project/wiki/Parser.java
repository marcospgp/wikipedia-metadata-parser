import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.HashMap;

import javax.xml.namespace.QName;
import javax.xml.stream.XMLEventReader;
import javax.xml.stream.XMLInputFactory;
import javax.xml.stream.XMLStreamException;
import javax.xml.stream.events.Attribute;
import javax.xml.stream.events.EndElement;
import javax.xml.stream.events.StartElement;
import javax.xml.stream.events.XMLEvent;

public class Parser {

  public static void parseWikiData(HashMap<Long, Article> articles, HashMap<Long, User> users, String[] args) {
    String fileName = args[0];
    parseXML(articles, users, fileName);

    // Testar recolha de dados de artigos
    for(Article curArticle : articles.values()){
      System.out.println(curArticle.toString()); // tenho de definir toString do articles
    }

    // Testar recolha de dados de users
    for(User curContributor : users.values()){
      System.out.println(curContributor.toString()); // tenho de definir toString do articles
    }
  }


  private static void parseXML(HashMap<Long, Article> articles, HashMap<Long, User> users, String fileName) {

    String title = null, revisionContributorUsername = null, revisionText = null, revisionTimestamp = null;
    long articleId = -1, revisionId = -1, revisionParentId = -1, revisionContributorId = -1;
    Article curArticle = null;
    User curUser = null;
    Revision curRevision = null;


    XMLInputFactory xmlInputFactory = XMLInputFactory.newInstance();
    try {
      XMLEventReader xmlEventReader = xmlInputFactory.createXMLEventReader(new FileInputStream(fileName));
      while(xmlEventReader.hasNext()){
        XMLEvent xmlEvent = xmlEventReader.nextEvent();
        if (xmlEvent.isStartElement()){
          StartElement startElement = xmlEvent.asStartElement();
          // COMEÇA NOVA PAGE
          if(startElement.getName().getLocalPart().equals("page")) {
            curArticle = new Article();
            curUser = new User();
          }
          else if(startElement.getName().getLocalPart().equals("title")) {
            xmlEvent = xmlEventReader.nextEvent();
            curArticle.setArticleTitle(xmlEvent.asCharacters().getData());
          }
          else if(startElement.getName().getLocalPart().equals("id")) {
            xmlEvent = xmlEventReader.nextEvent();
            curArticle.setArticleId(Long.parseLong(xmlEvent.asCharacters().getData()));
            curUser.setUserId(Long.parseLong(xmlEvent.asCharacters().getData()));
          }
          // PERCORRE A REVISION
          else if(startElement.getName().getLocalPart().equals("revision")){
            xmlEvent = xmlEventReader.nextEvent();
            curRevision = new Revision();
            while(xmlEventReader.hasNext()) {
              xmlEvent = xmlEventReader.nextEvent();
              if(startElement.getName().getLocalPart().equals("id")) {
                xmlEvent = xmlEventReader.nextEvent();
                curRevision.setRevisionId(Long.parseLong(xmlEvent.asCharacters().getData()));
              }
              else if(startElement.getName().getLocalPart().equals("parentid")) {
                    xmlEvent = xmlEventReader.nextEvent();
                    curRevision.setRevisionParentId(Long.parseLong(xmlEvent.asCharacters().getData()));
              }
              else if(startElement.getName().getLocalPart().equals("timestamp")) {
                    xmlEvent = xmlEventReader.nextEvent();
                    curRevision.setTimestamp(xmlEvent.asCharacters().getData());
              }
              // PERCORRE O CONTRIBUTOR
              else if(startElement.getName().getLocalPart().equals("contributor")) {
                    xmlEvent = xmlEventReader.nextEvent();
                    while(xmlEventReader.hasNext()) {
                      xmlEvent = xmlEventReader.nextEvent();
                      if(startElement.getName().getLocalPart().equals("username")) {
                        xmlEvent = xmlEventReader.nextEvent();
                        curUser.setUsername(xmlEvent.asCharacters().getData());
                      }
                      else if(startElement.getName().getLocalPart().equals("id")) {
                        xmlEvent = xmlEventReader.nextEvent();
                        curUser.setUserId(Long.parseLong(xmlEvent.asCharacters().getData()));
                      }
                    }
              }
            }
          }
        }

        //if Employee end element is reached, add employee object to list
        if(xmlEvent.isEndElement()){
          EndElement endElement = xmlEvent.asEndElement();
          if(endElement.getName().getLocalPart().equals("page")){
            articles.put(curArticle.getArticleId(), curArticle);
            users.put(curUser.getUserId(), curUser);
          }
        }
      }

    } catch (FileNotFoundException | XMLStreamException e) {
      e.printStackTrace();
    }
  }

}