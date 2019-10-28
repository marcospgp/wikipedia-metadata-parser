# wikipedia-metadata-parser

## What does it do?

 This C program parses Wikipedia XML metadata and exposes an interface that allows queries to be made regarding the data, such as the top contributors, biggest articles, and more.
 
## Results

 With the help of glib's hashtables, we were able to parse 1.6GB of data and test several queries on it in a little under 9.5 seconds (see results.txt).
