#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{
  const char *quotes[] = {
      "جب تم کسی چیز کو سچی طرح چاہو گے تو پوری کائنات تمہیں اسے حاصل کرنے میں مدد کرنے کے لئے متحرک ہوجائے گی۔ - رالف والڈو ایمرسن",
      "Life is like riding a bicycle. To keep your balance, you must keep moving. - Albert Einstein",
      "دنیا میں دو قسم کے لوگ ہوتے ہیں: ایک وہ جو تمہارے لئے خوشیاں لانے آتے ہیں اور دوسرے وہ جو تمہاری خوشیوں میں خوش ہوتے ہیں۔ - ویل سمیت",
      "The best way to predict the future is to create it. - Peter Drucker",
      "زندگی میں کامیابی کا راز یہ ہے کہ آپ کچھ کرنے کے لئے تیار ہوں جو دوسرے لوگ نہیں کرنا چاہتے۔ - آلان ہرسھفیلڈ",
      "The only limit to our realization of tomorrow will be our doubts of today. - Franklin D. Roosevelt"
      "Believe you can and you're halfway there. - Theodore Roosevelt",
      "ہمیشہ اپنے آپ پر بھروسہ رکھو۔ تم کر سکتے ہو۔ - ہیلن کیلر"};

  const int numQuotes = sizeof(quotes) / sizeof(quotes[0]);

  srand(time(0));

  int randomIndex = rand() % numQuotes;

  cout << "Random Quote of the Day:\n";
  cout << quotes[randomIndex] << endl;

  cin.ignore();
  cout << "\t\t\t" << "Press any key to Exit..." << endl;
  return 0;
  
  //cin.get();

  system("g++ -o deallocate dealloc_resource.cpp");
  // Run the deallocate resource file with an argument CALCULATOR
  system("./deallocate quotes");
}
