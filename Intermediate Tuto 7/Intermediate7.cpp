//Intermediate C++ Game Programming DirectX
//https://www.youtube.com/playlist?list=PLqCJpWy5Fohfil0gvjzgdV4h29R9kDKtZ

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


class DataBase {
public:
	//LAOD A TXT FILE CONTAINING A LIST OF WORDS
	void load(std::string filename_)
	{
		std::ifstream in(filename_);
		if (in) {
			for (std::string line; std::getline(in, line); )
			{
				//change the word to uppercase
				for (char& c : line)
				{
					c = toupper(c);
				}
				//push the word in the vector
				words.emplace_back(line);
			}
		}
	};
	
	std::vector<std::string> getWords()
	{
		return words;
	}
private:

	std::vector<std::string> words;
};

//check if the word has 5 letters
bool wordLenghtIsOk(const std::string word)
{
	if (word.length() == 5)
	{		
		return true;
	}
	else
	{		
		return false;
	}
};

//check if the word exists in the word list
bool wordExistsInDatabase(std::string word, std::vector<std::string> words)
{
	

	//Verifica se a palavra inserida existe no banco de palavras
	for (auto & w : words)
	{
		if (w == word)
		{
			return true;
		}
	}	
	return false;
}

//count how much points the guessed word sums
int countWordPoints(const std::string guessWord, const std::string secretWord, std::string & matchingChars)
{
	int points = 0;
	int guessWordPos = 0;//position of the char that is currently been checked in the guessWord
	int secretWordPos = 0;//position of the char that is currently been checked in the secretword
	
	bool matchPosition[5] = { false, false, false, false, false };

	for (const char & sc : secretWord)
	{		
		guessWordPos = 0;
		for (const char & gc : guessWord)
		{
			if (gc == sc && !matchPosition[secretWordPos])
			{
				points ++;				
				if (guessWordPos == secretWordPos)
				{
					points++;
					matchingChars[secretWordPos] = sc;
				}
				break;
			}
			guessWordPos++;
		}
		secretWordPos++;
	}

	return points;
}

//convert the word to uppercase
void wordToUpper(std::string& word)
{	
	for (char& c : word)
	{
		c = toupper(c);
	}
}

//LAOD THE LABELS OF THE GAME
void loadLabels(std::string fileName_, std::vector<std::string> & labels)
{
	std::ifstream in(fileName_);
	if (in)
	{
		for (std::string line; std::getline(in, line); )
		{
			labels.push_back(line);

		}
	}
}

//LOAD THE CORRESPONDING FILES FOR THE LANGUAGE CHOOSED BY THE USER
bool loadLanguage(const int language, std::vector<std::string> & labels, DataBase & db)
{
	switch (language)
	{
	case 0://PORTUGUÊS BR
		loadLabels("Pt-Br-Labels.txt", labels);
		db.load("db-pt-br.txt");
		return true;

	case 1://ENGLISH
		loadLabels("En-Label.txt", labels);
		db.load("db-en.txt");
		return true;

	default:
		return false;
	}
}
int main()
{
	/******************** TUTORIAL 9 - GUESSING GAME *************************/
	int language;
	
	DataBase * dt = new DataBase();//dataBase of words used by the game	
	std::vector<std::string> labels;//labels of the game
	std::vector<std::string> guessedWords; // list of words input by the user

	//REPEAT THE LOOP UNTIL THE USER INPUT A VALID VALUE FOR THE LANGUAGE
	do {
		std::cout << "ESCOLHA UM IDIOMA ( CHOOSE A LANGUAGE ) :\n( 0 ) PORTUGUÊS/n( 1 ) ENGLISH\n";
		std::cin >> language;
	} while (!loadLanguage(language, labels, *dt));
	
	std::string matchingChars = "#####";
	std::string secretWord = dt->getWords().at(rand() % dt->getWords().size());
	std::string guessWord = "empty";

	bool validWord = false;
	int roundPoints = 0;
	int totalPoints = 0;

	//RUNS THE GAME UNTIL THE USER INPUTS THE WORD 'EXIT'
	while (guessWord != /*SAIR*/ labels.at (0))
	{	
		//KEEP ASKING THE USER TO INPUT A WORD ULTIL IT'S A VALID ONE
		do{	
			std::cout << labels.at(1);
			std::cin >> guessWord;
			wordToUpper(guessWord);

			bool lengthOk = wordLenghtIsOk(guessWord);//CHECK IF IT'S A FIVE LETTERS WORD
			bool existInDb = wordExistsInDatabase(guessWord, dt->getWords());//CHECK IF THE WORD EXIST IN DB
			bool repeatedWord = wordExistsInDatabase(guessWord, guessedWords); //CHECK IF THE WORD ALREADY HAD BEEN GUESSED

			if (guessWord == /*SAIR*/ labels.at(0))//BREAK THE LOOP IF THE GUESSWORD IS 'EXIT'
			{
				break;
			}

			else if(!lengthOk)//THROW AN ERROR MESSAGE IF THE WORD IS LESS OR MORE THEN FIVE LETTERS
			{
				std::cout << guessWord << labels.at(2) << '\n';
			}

			else if (!existInDb)//THROW A ERROR MESSAGE IF THE WORD DOESN'T EXISTS IN DB
			{
				std::cout << guessWord << labels.at(3) << '\n';
			}

			else if (repeatedWord)//THROW A ERROR MESSAGE IF IT'S A REPEATED WORD
			{
				std::cout << guessWord << labels.at(4) << '\n';
			}

			validWord = lengthOk && existInDb && !repeatedWord;
		}while (!validWord);

		if (validWord)
		{
			guessedWords.push_back(guessWord);
			if (guessWord == secretWord)
			{
				std::cout << labels.at(5);
				guessWord = labels.at(0);
				std::cin.get();
			}
			else
			{				
				roundPoints = countWordPoints(guessWord, secretWord, matchingChars);
				totalPoints += roundPoints;
				std::cout << labels.at(6) << matchingChars << '\n';
				std::cout << labels.at(7) << roundPoints << labels.at(8) << guessWord << '\n' << labels.at(9) << totalPoints << '\n';
			}
		}
	}
	std::cin.get();
	/**************** END OF TUTORIAL 9 - GUESSING GAME **********************/
	return 0;
}