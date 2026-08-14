#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

class BigInt {
    string number;    // Stores the number as a string
    bool isNegative;  // True if number is negative

    // Remove unnecessary leading zeros from the number string
    void removeLeadingZeros() {
    int i = 0;

    while (i < number.length() - 1 && number[i] == '0') {
        i++;
    }

    number.erase(0, i);

    if (number == "0") {
        isNegative = false;
    }
}

    // Compare absolute values of two BigInts (ignore signs)
    // Returns: 1 if |this| > |other|, 0 if equal, -1 if |this| < |other|
    int compareMagnitude(const BigInt& other) const {

    // First compare the number of digits
    if (number.length() > other.number.length()) {
        return 1;
    }

    if (number.length() < other.number.length()) {
        return -1;
    }

    // Same number of digits
    if (number > other.number) {
        return 1;
    }

    if (number < other.number) {
        return -1;
    }

    // Both numbers are equal
    return 0;
}

private:
    BigInt divideByTwo(const BigInt& value) const; // Helper function to divide a BigInt by 2

public:
    // Default constructor - initialize to zero
   BigInt() {
    number = "0";
    isNegative = false;
}

    // Constructor from 64-bit integer
    BigInt(int64_t value) {

    if (value < 0) {
        isNegative = true;

        // Convert number to string
        number = to_string(value);

        // Remove the '-'
        number = number.substr(1);
    }
    else {
        isNegative = false;
        number = to_string(value);
    }

    removeLeadingZeros();
}

    // Constructor from string representation
  BigInt(const string& str) {

    if (str.empty()) {
        number = "0";
        isNegative = false;
    }
    else if (str[0] == '-') {
        isNegative = true;
        number = str.substr(1);
    }
    else {
        isNegative = false;
        number = str;
    }

    removeLeadingZeros();
}

    // Copy constructor
    BigInt(const BigInt& other) {
    number = other.number;
    isNegative = other.isNegative;
}

    // Destructor
    ~BigInt() {
        // TODO: Implement if needed
    }

    // Assignment operator
    BigInt& operator=(const BigInt& other) {

    if (this == &other) {
        return *this;
    }

    number = other.number;
    isNegative = other.isNegative;

    return *this;
}

    // Unary negation operator (-x)
    BigInt operator-() const {
        BigInt result;
        // TODO: Implement negation logic
        return result;
    }

    // Unary plus operator (+x)
    BigInt operator+() const {
        BigInt result;
        // TODO: Implement this operator
        return result;
    }

    // Addition assignment operator (x += y)
    BigInt& operator+=(const BigInt& other) {
        string result = "";
        //Add magnitudes if both have same signs using carry logic
        if (this->isNegative == other.isNegative) {
            int carry = 0, sum = 0;
            int i = this -> number.length() - 1;
            int j = other.number.length() - 1;
            //This while loop condition makes sure that you iterate through all numbers and if there are carries to be added to final result
            while (i >= 0 || j >= 0 || carry > 0) {
                int sum = carry;
                
                //Make sure we aren't going out of bounds from both numbers by using these 2 if conditions at first
                if (i >= 0) {
                    sum += this->number[i] - '0';
                    i--;
                }
                
                if (j >= 0) {
                    sum += other.number[j] - '0';
                    j--;
                }
                
                carry = sum / 10;
                result = char((sum % 10) + '0') + result;
            }
            this -> number = result;
        } 
        //If adding different sign numbers do subtraction instead using borrow logic
        else {
            int comparison = this -> compareMagnitude(other);
            int borrow = 0, diff = 0;
            //If numbers are equal in magnitude
            if(comparison == 0){
                this -> number = "0";
                this -> isNegative = 0;
                return *this;
            }

            string topString, bottomString;
            //If this numbers is greater than other
            if(comparison == 1){
                topString = this -> number;
                bottomString = other.number;
            }
            //If other number is greater than this
            else{
                this -> isNegative = other.isNegative;
                topString = other.number;
                bottomString = this -> number;
            }

            int t = topString.length() - 1;
            int b = bottomString.length() - 1;

            while(t >= 0){
                diff = (topString[t] - '0') - borrow;
                //Making sure with this if condtion that we aren't going out of bounds
                if(b >= 0){
                    diff -= (bottomString[b] - '0');
                    b--;
                }

                if(diff < 0){
                    diff += 10;
                    borrow = 1;
                }
                else{
                    borrow = 0;
                }
                //Not += here so that it appends from the front
                result = char(diff + '0') + result;
                t--;
            }
            
            this -> number = result;
        }

        this -> removeLeadingZeros();
        return *this;
    }

    // Subtraction assignment operator (x -= y)
    BigInt& operator-=(const BigInt& other) {
        //Making temporary copy of the number
        BigInt temp = other;
        //Then flip the number's sign if it isn't zero so we can add instead of subtract
        if(temp.number != "0"){
            temp.isNegative = !temp.isNegative;
        }
        //If the if condition was skipped the magnitudes will be added as normal while retaining signs 
        *this += temp;
        return *this;
    }


    // Multiplication assignment operator (x *= y)
    BigInt& operator*=(const BigInt& other) {

        //check if either number is zero
        if (number == "0" || other.number == "0")
        {
            number = "0";
            isNegative = false;
            return *this;
        }

        //create a vector to store result of multiplication in it 
        vector<int>result(number.length() + other.number.length(), 0);

        for (int i = other.number.length() - 1; i >= 0; i--) {

            for (int j = number.length() - 1; j >= 0; j--) {

                int p1 = i + j + 1;  // position of current digit in vector
                int p2 = i + j;     //position of carry in vector 

                int product = (number[j] - '0') * (other.number[i] - '0');    //multiply current digits
                int sum = product + result[p1];   //add any value already exists in result[p1] 
                
                result[p1] = sum % 10; //store current digit
                result[p2] += sum / 10; //add carry to previous position 
            }
        }
        //convert vector to string

        number = ""; //to update number by the result of multiplication
        for (int digit : result) {
            number += digit + '0'; //convert integer numbers inside the vector to characters and add it to the string number
        }
        removeLeadingZeros(); 

        if (isNegative == other.isNegative)
        {
            isNegative = false;
        }
        else
        {
            isNegative = true;
        }
        return *this;
    }

    // Division assignment operator (x /= y)
    BigInt& operator/=(const BigInt& other) {
        // Check for division by zero
        if(other.number == "0")
        {
            throw runtime_error("Division by zero");
        }
        //Magnitude comparison
        if(this->compareMagnitude(other) < 0)
        {
            return *this = BigInt(0);
        }

        //used in binary search
        BigInt dividend = *this;
        BigInt divisor = other;

        //store signs for later
        bool dividendSign = dividend.isNegative;
        bool divisorSign = divisor.isNegative;

        //remove signs for division
        dividend.isNegative = false;
        divisor.isNegative = false;

        //binary search for quotient
        BigInt left = BigInt(0);
        BigInt right = dividend;
        while(left<=right)
        {
            BigInt mid = divideByTwo(left + right);
            BigInt product = mid * divisor;
            if(product <= dividend && (mid + BigInt(1)) * divisor > dividend)
            {
                *this = mid;
                break;
            }
            else if(product < dividend)
            {
                left = mid + BigInt(1);
            }
            else
            {
                right = mid - BigInt(1);
            }
        }

        //sign handling
        if(dividendSign ^ divisorSign)
        {
            this->isNegative = true;
        }
        else
        {
            this->isNegative = false;
        }

        // Remove leading zeros from the result
        this->removeLeadingZeros();

        return *this;
    }

    // Modulus assignment operator (x %= y)
    BigInt& operator%=(const BigInt& other) {
        // TODO: Implement this operator
        return *this;
    }

    // Pre-increment operator (++x)
BigInt& operator++() {
    *this += BigInt(1);
    return *this;
}

    // Post-increment operator (x++)
BigInt operator++(int) {
    BigInt temp = *this;
    *this += BigInt(1);
    return temp;
}
    // Pre-decrement operator (--x)
BigInt& operator--() {
    *this -= BigInt(1);
    return *this;
}

    // Post-decrement operator (x--)
BigInt operator--(int) {
    BigInt temp = *this;
    *this -= BigInt(1);
    return temp;
}

    // Convert BigInt to string representation
    string toString() const {
        // TODO: Implement this function
        return "";
    }

    // Output stream operator (for printing)
    friend ostream& operator<<(ostream& os, const BigInt& num) {
        // TODO: Implement this operator
        return os;
    }

    // Input stream operator (for reading from input)
    friend istream& operator>>(istream& is, BigInt& num) {
        // TODO: Implement this operator
        return is;
    }

    // Friend declarations for comparison operators
    friend bool operator==(const BigInt& lhs, const BigInt& rhs);
    friend bool operator<(const BigInt& lhs, const BigInt& rhs);

    // Friend declarations for arithmetic operators
    friend BigInt operator/(BigInt lhs, const BigInt& rhs);
};

BigInt BigInt::divideByTwo(const BigInt& value) const
{
    string quotient;
    int remainder = 0;

    for (char c : value.number)
    {
        int current = remainder * 10 + (c - '0');
        int quotientDigit = current / 2;
        remainder = current % 2;
        quotient += char(quotientDigit + '0');
    }

    BigInt result(quotient);
    result.removeLeadingZeros();

    return result;
}

// Binary addition operator (x + y)
BigInt operator+(BigInt lhs, const BigInt& rhs) {
    BigInt result = lhs;
    result += rhs;
    return result;
}

// Binary subtraction operator (x - y)
BigInt operator-(BigInt lhs, const BigInt& rhs) {
    BigInt result = lhs;
    result -= rhs;
    return result;
}

// Binary multiplication operator (x * y)
BigInt operator*(BigInt lhs, const BigInt& rhs) {
    BigInt result = lhs;
    result *= rhs;
    return result;
}

// Binary division operator (x / y)
BigInt operator/(BigInt lhs, const BigInt& rhs) {
    BigInt result = lhs /= rhs; // from the division assignment operator
    return result;
}

// Binary modulus operator (x % y)
BigInt operator%(BigInt lhs, const BigInt& rhs) {
    BigInt result;
    // TODO: Implement this operator
    return result;
}

// Equality comparison operator (x == y)
bool operator==(const BigInt& lhs, const BigInt& rhs) {
    if (lhs.number == rhs.number &&
        lhs.isNegative == rhs.isNegative) {
        return true;
    }

    return false;
}

// Inequality comparison operator (x != y)
bool operator!=(const BigInt& lhs, const BigInt& rhs) {
    return !(lhs == rhs);
}

// Less-than comparison operator (x < y)
bool operator<(const BigInt& lhs, const BigInt& rhs) {

    // If signs are different
    if (lhs.isNegative != rhs.isNegative) {
        return lhs.isNegative;
    }

    // Both numbers are positive
    if (!lhs.isNegative) {
        return lhs.compareMagnitude(rhs) < 0;
    }

    // Both numbers are negative
    return lhs.compareMagnitude(rhs) > 0;
}

// Less-than-or-equal comparison operator (x <= y)
bool operator<=(const BigInt& lhs, const BigInt& rhs) {
    return (lhs < rhs) || (lhs == rhs);
}

// Greater-than comparison operator (x > y)
bool operator>(const BigInt& lhs, const BigInt& rhs) {
    return !(lhs <= rhs);
}

// Greater-than-or-equal comparison operator (x >= y)
bool operator>=(const BigInt& lhs, const BigInt& rhs) {
    return !(lhs < rhs);
}

int main() {
    cout << "=== BigInt Class Test Program ===" << endl << endl;
    cout << "NOTE: All functions are currently empty." << endl;
    cout << "Your task is to implement ALL the functions above." << endl;
    cout << "The tests below will work once you implement them correctly." << endl << endl;

    /*
    // Test 1: Constructors and basic output
    cout << "1. Constructors and output:" << endl;
    BigInt a(12345);              // Should create BigInt from integer
    BigInt b("-67890");           // Should create BigInt from string
    BigInt c("0");                // Should handle zero correctly
    BigInt d = a;                 // Should use copy constructor
    cout << "a (from int): " << a << endl;        // Should print "12345"
    cout << "b (from string): " << b << endl;     // Should print "-67890"
    cout << "c (zero): " << c << endl;            // Should print "0"
    cout << "d (copy of a): " << d << endl << endl; // Should print "12345"

    // Test 2: Arithmetic operations
    cout << "2. Arithmetic operations:" << endl;
    cout << "a + b = " << a + b << endl;          // Should calculate 12345 + (-67890)
    cout << "a - b = " << a - b << endl;          // Should calculate 12345 - (-67890)
    cout << "a * b = " << a * b << endl;          // Should calculate 12345 * (-67890)
    cout << "b / a = " << b / a << endl;          // Should calculate (-67890) / 12345
    cout << "a % 100 = " << a % BigInt(100) << endl << endl; // Should calculate 12345 % 100

    // Test 3: Relational operators
    cout << "3. Relational operators:" << endl;
    cout << "a == d: " << (a == d) << endl;       // Should be true (12345 == 12345)
    cout << "a != b: " << (a != b) << endl;       // Should be true (12345 != -67890)
    cout << "a < b: " << (a < b) << endl;         // Should be false (12345 < -67890)
    cout << "a > b: " << (a > b) << endl;         // Should be true (12345 > -67890)
    cout << "c == 0: " << (c == BigInt(0)) << endl << endl; // Should be true (0 == 0)

    // Test 4: Unary operators and increments
    cout << "4. Unary operators and increments:" << endl;
    cout << "-a: " << -a << endl;                 // Should print "-12345"
    cout << "++a: " << ++a << endl;               // Should increment and print "12346"
    cout << "a--: " << a-- << endl;               // Should print "12346" then decrement
    cout << "a after decrement: " << a << endl << endl; // Should print "12345"

    // Test 5: Large number operations
    cout << "5. Large number operations:" << endl;
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");
    cout << "Very large addition: " << num1 + num2 << endl;
    cout << "Very large multiplication: " << num1 * num2 << endl << endl;

    // Test 6: Edge cases and error handling
    cout << "6. Edge cases:" << endl;
    BigInt zero(0);
    BigInt one(1);
    try {
        BigInt result = one / zero;               // Should throw division by zero error
        cout << "Division by zero succeeded (unexpected)" << endl;
    } catch (const runtime_error& e) {
        cout << "Division by zero correctly threw error: " << e.what() << endl;
    }
    cout << "Multiplication by zero: " << one * zero << endl;        // Should be "0"
    cout << "Negative multiplication: " << BigInt(-5) * BigInt(3) << endl;  // Should be "-15"
    cout << "Negative division: " << BigInt(-10) / BigInt(3) << endl;       // Should be "-3"
    cout << "Negative modulus: " << BigInt(-10) % BigInt(3) << endl;        // Should be "-1"
    */

    return 0;
}
