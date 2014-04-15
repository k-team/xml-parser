# Validation d'un document

Un document est validé grâce à un fichier XSD.

Le document XSD est d'abord transformé en graphe nommée d'expressions
régulières. Ce graphe est modélisé par une *std::map* de *std::string* vers
*Node*. L'index représente le nom du tag XML et la valeur, les conditions de
validation de ce tag. Ce dernier est développer par une structure C++ avec 3
*std:string* :

- **reg_tag** : Expression régulière sur les tags intérieurs
- **reg_attr** : Expression régulière sur les attributs (non utilisé
  finalement)
- **reg_content** : Expression régulière sur le contenu

La construction du graphe se fait en parcourant uniquement le document XSD. Une
autre *std::map* est fourni pour contenir les types déclarés pendant la
construction pour gérer les tags *complexeType*. Afin de gérer les déclarations
des types après leur utilisation, la construction est faite deux fois.

Voici l'algorithme de construction du graphe :

    fonction construct_schema(SchemaTag S, Graph G, Type T)
        reg_tag = ""
        Pour chaque tag t de S ayant pour nom "element"
            Si t a un attribut "name"
                reg_tag += "<" + t.attributes["name"] + ">|"
                Si t est un tag inline
                    construct_empty_tag(t, G, T)
                Sinon
                    construct_composite_tag(t, G, T)
            Sinon si t a un attribut "ref"
                min = t.attributes["minOccurs"] ou "1"
                max = t.attributes["maxOccurs"] ou "1"
                Si max = "unbounded"
                    max = ""
                reg_tag += "(<" + t.attributes["ref"] + ">){" + min + "," + max +
                        "}|"
        Pour chaque tag t de S ayant pour nom "complexType"
            construct_complex_type(t, G, T)
        Si reg_tag not empty
            reg_tag.pop_last() # On enlève le dernier caractère |
        G[""] = Node(reg_tag=reg_tag) # Représente la racine du graphe

    fonction construct_empty_tag(EmptyTag E, Graph G, Type T)
        node = Node()
        Si E.attributes["type"] = "string"
            node.reg_content = REGEX_STRING
        Sinon si E.attributes["type"] = "date"
            node.reg_content = REGEX_DATE
        Sinon si E.attributes["type"] dans T
            node = T[E.attributes["type"]]
        G[T.name] = node

    fonction construct_composite_tag(CompositeTag C, Graph G, Type T)
        Si premier element P de C est nommé "complexType"
            G[C.name] = construct_complex_type(P, G, T)

    fonction construct_complex_type(ComplexTypeTag C, Graph G, Type T)
        Si premier element P de C est nommé "sequence"
            node = construct_sequence(P, G, T)
        Si premier element P de C est nommé "choice"
            node = construct_choice(P, G, T)
        Si C a l'attribut "name"
            T[C.attributes["name"]] = node
        retourner node

    fonction construct_sequence(SequenceTag S, Graph G, Type T)
        reg_tag = ""
        Pour chaque tag t de S ayant pour nom "element"
            Si t a un attribut "name"
                reg_tag += "<" + t.attributes["name"] + ">"
                Si t est un tag inline
                    construct_empty_tag(t, G, T)
                Sinon
                    construct_composite_tag(t, G, T)
            Sinon si t a un attribut "ref"
                min = t.attributes["minOccurs"] ou "1"
                max = t.attributes["maxOccurs"] ou "1"
                Si max = "unbounded"
                    max = ""
                reg_tag += "(<" + t.attributes["ref"] + ">){" + min + "," + max +
                        "}"
        retourner Node(reg_tag=reg_tag)

    fonction construct_choice(ChoiceTag S, Graph G, Type T)
        reg_tag = ""
        Pour chaque tag t de S ayant pour nom "element"
            Si t a un attribut "name"
                reg_tag += "<" + t.attributes["name"] + ">|"
                Si t est un tag inline
                    construct_empty_tag(t, G, T)
                Sinon
                    construct_composite_tag(t, G, T)
            Sinon si t a un attribut "ref"
                min = t.attributes["minOccurs"] ou "1"
                max = t.attributes["maxOccurs"] ou "1"
                Si max = "unbounded"
                    max = ""
                reg_tag += "(<" + t.attributes["ref"] + ">){" + min + "," + max +
                        "}|"
        Si reg_tag not empty
            reg_tag.pop_last() # On enlève le dernier caractère |
        retourner Node(reg_tag=reg_tag)
