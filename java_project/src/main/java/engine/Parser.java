package engine;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.Iterator;
import java.util.ArrayList;
import java.util.List;
import java.util.HashMap;

import javax.xml.stream.XMLStreamReader;
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
			// System.out.println(curArticle.toString()); // tenho de definir toString do articles
		}

		// Testar recolha de dados de users
		/*
		for(User curContributor : users.values()){
			// System.out.println(curContributor.toString()); // tenho de definir toString do articles
		}
		*/
		}


		private static void parseXML(HashMap<Long, Article> articles, HashMap<Long, User> users, String fileName) {

		String title = null, revisionContributorUsername = null, revisionText = null, revisionTimestamp = null;
		long articleId = -1L, revisionId = -1L, revisionParentId = -1L, revisionContributorId = -1L;
		boolean onContributor = false, onRevision = false;


		// System.out.println("Comecei o parse.\n");

		try {
			XMLInputFactory factory = XMLInputFactory.newInstance();
			XMLStreamReader xmlStreamReader = factory.createXMLStreamReader(new FileInputStream(fileName));

			while(xmlStreamReader.hasNext()) {

				int event = xmlStreamReader.getEventType();

				if (event == XMLStreamConstants.START_ELEMENT) {

					if (xmlStreamReader.getLocalName().equals("page")) {

						title = null;
						revisionContributorUsername = null;
						revisionText = null;
						revisionTimestamp = null;

						articleId = -1L;
						revisionId = -1L;
						revisionParentId = -1L;
						revisionContributorId = -1L; // Resetar revisionContributorId

						// System.out.println("Start Element : page");

					} else if (xmlStreamReader.getLocalName().equals("title")) {

						title = xmlStreamReader.getElementText();
						// System.out.println("Title: " + title);

					} else if (xmlStreamReader.getLocalName().equals("revision")) {

						onRevision = true;

						// System.out.println("Start Element : revision");

					} else if (xmlStreamReader.getLocalName().equals("parentid")) {

						revisionParentId = Long.parseLong(xmlStreamReader.getElementText());
						// System.out.println("Revision Parent id: " + revisionParentId);

					} else if (xmlStreamReader.getLocalName().equals("timestamp")) {

						revisionTimestamp = xmlStreamReader.getElementText();
						// System.out.println("Revision timestamp: " + revisionTimestamp);

					} else if (xmlStreamReader.getLocalName().equals("contributor")) {

						onContributor = true;

						// System.out.println("Start Element : contributor");

					} else if (xmlStreamReader.getLocalName().equals("username")) {

						revisionContributorUsername = xmlStreamReader.getElementText();
						// System.out.println("User name: " + revisionContributorUsername);

					} else if (xmlStreamReader.getLocalName().equals("id") && onContributor) {

						revisionContributorId = Long.parseLong(xmlStreamReader.getElementText());
						// System.out.println("User id: " + revisionContributorId);

					} else if (xmlStreamReader.getLocalName().equals("id") && onRevision) {

						revisionId = Long.parseLong(xmlStreamReader.getElementText());
						// System.out.println("Revision id: " + revisionId);

					} else if (xmlStreamReader.getLocalName().equals("id")) {

						articleId = Long.parseLong(xmlStreamReader.getElementText());
						// System.out.println("Article id: " + articleId);

					} else if (xmlStreamReader.getLocalName().equals("text")) {

						revisionText = xmlStreamReader.getElementText();
						// System.out.println("Text: " + revisionText);
					}

				} else if (event == XMLStreamConstants.END_ELEMENT) {

					if (xmlStreamReader.getLocalName().equals("page")) {

						// Ignorar contributor caso apenas tenha <ip> e não <username> + <id>
						if (revisionContributorId != -1L) {

							//System.out.println("Calling onPageUsers");
							Users.onPageUsers(users, articles, revisionContributorId, revisionContributorUsername, articleId, revisionId);
						}

						//System.out.println("Calling onPageArticles");

						if (articleId == 39) {
							System.out.println("Sending article 39");
						}

						Articles.onPageArticles(articles, articleId, title, revisionText, revisionId, revisionParentId, revisionTimestamp);
						// System.out.println("End Element : page");
						// System.out.println();

					} else if (xmlStreamReader.getLocalName().equals("revision")) {

						onRevision = false;

					} else if (xmlStreamReader.getLocalName().equals("contributor")) {

						onContributor = false;
					}
				}

				event = xmlStreamReader.next();
			}

		} catch (FileNotFoundException e) {

			e.printStackTrace();

		} catch (XMLStreamException e) {

			e.printStackTrace();

		}
	}
}
