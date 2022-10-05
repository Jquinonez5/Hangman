#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <string>

using namespace std;

class Words
{
    private:
        int minlen;
        int maxlen;
        int count;
        string * choices;

        int count_candidates()
        {
            ifstream fin;

            char line[50];
            int counter;

            fin.open("enable1.txt");

            if(fin.is_open())
            {

                while (!fin.eof())
                {
                    fin >> line;

                    if (fin.good())
                    {
                        if (strlen(line) >= minlen && strlen(line) <= maxlen)
                        {
                            counter++;
                        }
                    }
                }
            }

            fin.close();

            return counter;
        }

        void load_words()
        {

            ifstream fin;

            fin.open("enable1.txt");

            if(fin.is_open())
            {

                choices = new string[count];
                char line[50];
                int counter = 0;

                while (!fin.eof())
                {
                    fin >> line;


                    if (fin.good())
                    {
                        if (strlen(line) >= minlen && strlen(line) <= maxlen)
                        {
                            choices[counter] = line;
                            counter++;
                        }
                    }
                }
            }


            fin.close();
        }

    public:
        Words(int minl, int maxl)
        {
            minlen = minl;
            maxlen = maxl;
            count = count_candidates();
            load_words();
        }

        string pick_word()
        {
            string word;

            if (count == 0)
            {
                word = "";
            }
            else 
            {
                word = choices[rand() % count + 1];
            }


            return word;
        }

        void reset(int min, int max)
        {
            minlen = min;
            maxlen = max;

            if(choices != NULL)
            {
                delete [] choices;
            }

            count_candidates();
           load_words();
        }

        ~Words()
        {

            delete [] choices;
        }

};

class Hangman
{
    private:
        char word[40], progress[40];
        int word_length;

        void clear_progress(int length)
        {
            for(int i = 0; i < length; i++)
            {
                progress[i] = '-';
            }

            progress[length] = '\0';
        }

    protected:
        int matches;
        char last_guess;
        string chars_guessed;
        int wrong_guesses;
        int remaining; 
        bool exists;

        bool check(char user_guess)
        {
            exists = false;
            matches = 0;
            bool is_correct = false;
            last_guess = user_guess;

            for(int i = 0; i < word_length; i++)
            {
                if(word[i] == user_guess)
                {
                    is_correct = true;
                    progress[i] = user_guess;
                    matches++;
                }
            }

            if(is_correct == false)
            {

                for(int i = 0; i < chars_guessed.length(); i++)
                {
                    if(chars_guessed[i] == user_guess)
                    {
                        exists = true;
                    }
                }

                if(exists == false)
                {
                    chars_guessed += user_guess;
                    wrong_guesses++;
                    remaining--;
                }

            }

            return is_correct;

        }

    public:   
        void initialize(string start)
        {
            chars_guessed = " ";
            wrong_guesses = 0;
            remaining = 6;
            strcpy(word, start.c_str());
            word_length = start.length();
            clear_progress(word_length);
        }

        Hangman(string hm)
        {
            initialize(hm);
        } 

        char * get_progress()
        {
            return progress;
        }

        char * get_word()
        {
            return word;
        }

        int get_remaining()
        {
            return remaining;
        }

        bool is_word_complete()
        {
            bool complete = false;

            if(strcmp(progress, word) == 0)
            {
                complete = true;
            }

            return complete;
        }
};

class HangmanConsole : public Hangman
{
    public:
        HangmanConsole(string start) : Hangman(start)
    {
    }

        void show_status(int stage)
        {
            switch (stage)
            {
                case 0:
                    cout << " +-----\n";
                    cout << " |     \n";
                    cout << " |     \n";
                    cout << " |     \n";
                    cout  << " |     \n";
                    cout << " ----- \n";
                    break;

                case 1:
                    cout << " +-----\n";
                    cout << " |  0   \n";
                    cout << " |     \n";
                    cout << " |     \n";
                    cout << " |     \n";
                    cout << " ----- \n";
                    break;

                case 2:
                    cout << " +-----\n";
                    cout << " |  0   \n";
                    cout << " |  |\\   \n";
                    cout << " |     \n";
                    cout << " |     \n";
                    cout << " ----- \n";
                    break;

                case 3:
                    cout << " +-----\n";
                    cout << " |  0   \n";
                    cout << " | /|\\   \n";
                    cout << " |     \n";
                    cout << " |     \n";
                    cout << " ----- \n";
                    break;

                case 4:
                    cout << " +-----\n";
                    cout << " |  0   \n";
                    cout << " | /|\\   \n";
                    cout << " |  |   \n";
                    cout << " |     \n";
                    cout << " ----- \n";
                    break;

                case 5:
                    cout << " +-----\n";
                    cout << " |  0   \n";
                    cout << " | /|\\    \n";
                    cout << " |  |   \n";
                    cout << " | /    \n";
                    cout << " ----- \n";
                    break;

                case 6:
                    cout << " +-----\n";
                    cout << " |  0   \n";
                    cout << " | /|\\    \n";
                    cout << " |  |   \n";
                    cout << " | / \\   \n";
                    cout << " ----- \n";
                    break;
            }
        }

        void show_info()
        {
            show_status(wrong_guesses);
            cout << get_progress() << endl;
            cout << "Guess: " << last_guess << endl;
            cout << "Matches: " << matches << endl;
            cout << "Wrong Guesses: " << wrong_guesses << endl;
            cout << "Attempts Remaing: " << remaining << endl;
            cout << "Guessed: " << chars_guessed << endl;
        }

        friend istream & operator>>(istream & in, HangmanConsole & g);
};

istream & operator>>(istream & in, HangmanConsole & hc)
{
    char input;

    in >> input;
    hc.check(input);

    return in;
}

int show_menu()
{
    int choice;

    cout << "1. Play the computer (auto select random range)" << endl;
    cout << "2. Play the computer (min: 7, max: 12)" << endl;
    cout << "3. Play the computer (user selects/changes range)" << endl;
    cout << "4. Play the computer (use same range stored in 3)" << endl;
    cout << "5. Two player mode (user1 enters the word, user2 guesses)" << endl;
    cout << "6. Quit" << endl << endl;

    cout << "Enter choice: ";
    cin >> choice;

    return choice;
}

void get_range_num(string prompt, int min, int max, int & number)
{
    int user_num;

    cout << prompt << endl;
    cin >> user_num;

    if(user_num >= min && user_num <= max)
    {
        number = user_num;
    }
    else 
    {
        get_range_num(prompt, min, max, number);
    }
}

int main()
{
    int choice;
    int min;
    int max;
    bool option = false;
    string one_word;
    string input;
    bool two_players;

    srand(time(NULL)); // needs <ctime> included


    while(choice != 6)
    {
        two_players = false;

        choice = show_menu();


        if(choice == 3)
        {
            option = true;
        }

        switch(choice)
        {
            case 1: max = rand() % 40 + 1;
                    min = rand() % max;
                    break;

            case 2: max = 12;
                    min = 7;
                    break;

            case 3: get_range_num("Enter min: ", 2, 40, min);
                    get_range_num("Enter max: ", min, 40, max); 
                    break;

            case 4: if(option == false)
                    {
                        cout << "Choice 3 has not been given a value yet" << endl;
                        get_range_num("Enter min: ", 2, 40, min);
                        get_range_num("Enter max: ", min, 40, max); 
                    }
                    break;

            case 5: cout << "Player 1 enter word";
                    cin >> one_word;
                    system("clear");
                    two_players = true; 
        } 

        Words words(min, max); // words between 7 and 10 chars long


        if(two_players == true)
        {
            input = one_word;
        }
        else
        {
            input = words.pick_word();
        }

        if(choice != 6)
        {
            HangmanConsole game(input);  


            cout << "HANGMAN" << endl << "-------" << endl << endl;
            cout << "You word is: " << game.get_progress() << endl;

            while (!game.is_word_complete() && game.get_remaining() > 0)
            {
                cout << endl;
                cout << "Enter your guess: ";
                cin >> game;

                system("clear"); // NON-PORTABLE, ONLY WORKS ON LINUX
                game.show_info();
            }

        }
    }

    return 0;
}
