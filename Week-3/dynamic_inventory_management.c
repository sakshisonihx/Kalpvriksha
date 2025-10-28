#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define NAMELENGTH 50
#define MAXPRICE 100000
#define MAXQTY 1000000
#define MAXID 10000

typedef struct
{
    int productID;
    char productName[NAMELENGTH];
    float price;
    int quantity;
} product;

product *inventory;
int totalProducts;

void addProduct();
void displayProducts();
void updateQuantity();
void searchByID();
void searchByName();
void searchByPriceRange();
void deleteProduct();
void clearInput();
char *searchString(char[], char[]);
int checDublicateID(int);

int main()
{
    printf("Enter initial number of products (1-100): ");
    int inputCheck = scanf("%d", &totalProducts);
    if (inputCheck != 1)
    {
        printf("Error. Enter integer");
        return 1;
    }
    if (totalProducts > 100 || totalProducts < 0)
    {
        printf("Error. Initial amount must be between 1 to 100");
        return 1;
    }

    inventory = (product *)calloc(totalProducts, sizeof(product));

    for (int currentIndex = 0; currentIndex < totalProducts; currentIndex++)
    {
        int newID = 0, newQuantity = -1;
        float newPrice = -1;
        printf("Enter details of product %d\n", currentIndex + 1);
        printf("Product ID: ");
        inputCheck = scanf("%d", &newID);
        if (inputCheck != 1)
        {
            printf("Error. Enter integer");
            return 1;
        }
        else if (newID < 1 || newID > MAXID)
        {
            printf("Error. ID must be between 1 and %d", MAXID);
            return 1;
        }
        else if (!checDublicateID(newID))
        {
            printf("Error. Dublicate ID entered");
            return 1;
        }
        (inventory + currentIndex)->productID = newID;

        clearInput();
        printf("Product Name: ");
        inputCheck = scanf("%49[^\n]", (inventory + currentIndex)->productName);
        if (inputCheck != 1)
        {
            printf("Error. Enter string");
            return 1;
        }

        printf("Product Price: ");
        inputCheck = scanf("%f", &newPrice);
        if (inputCheck != 1)
        {
            printf("Error. Enter a number");
            return 1;
        }
        else if (newPrice < 0 || newPrice > MAXPRICE)
        {
            printf("Error. Price must be between 0 and %d", MAXPRICE);
            return 1;
        }
        (inventory + currentIndex)->price = newPrice;

        printf("Product Quantity: ");
        inputCheck = scanf("%d", &newQuantity);
        if (inputCheck != 1)
        {
            printf("Error. Enter a number");
            return 1;
        }
        else if (newQuantity < 0 || newQuantity > MAXQTY)
        {
            printf("Error. Quantity must be between 0 and %d", MAXQTY);
            return 1;
        }
        (inventory + currentIndex)->quantity = newQuantity;
    }
    int choice = 0;
    do
    {
        printf("\n\n===== INVENTORY MANAGEMENT MENU =====\n");
        printf("1. Add new product.\n");
        printf("2. View all products.\n");
        printf("3. Update quantity of a product.\n");
        printf("4. Search products by ID.\n");
        printf("5. Search products by name.\n");
        printf("6. Search products by price range.\n");
        printf("7. Delete a product.\n");
        printf("8. Exit.\n");
        printf("your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            addProduct();
            break;
        case 2:
            displayProducts();
            break;
        case 3:
            updateQuantity();
            break;
        case 4:
            searchByID();
            break;
        case 5:
            searchByName();
            break;
        case 6:
            searchByPriceRange();
            break;
        case 7:
            deleteProduct();
            break;
        case 8:
            printf("Exit");
            free(inventory);
            break;
        default:
            printf("Invalid choice.");
        }
    } while (choice != 8);

    return 0;
}

// consume '\n'
void clearInput()
{
    int inputChar;
    while ((inputChar = getchar()) != '\n' && inputChar != EOF)
        ;
}

// helper function to search for partial string
char *searchString(char mainString[], char matchString[])
{
    if (*matchString == '\0') // empty seach string
    {
        return mainString;
    }

    for (int mainIndex = 0; mainString[mainIndex] != '\0'; mainIndex++)
    {
        int searchIndex = 0;
        while (matchString[searchIndex] != '\0' &&
               mainString[mainIndex + searchIndex] != '\0' &&
               toupper(mainString[mainIndex + searchIndex]) == toupper(matchString[searchIndex]))
        {
            searchIndex++;
        }
        if (matchString[searchIndex] == '\0') // match found
        {
            return (mainString + mainIndex);
        }
    }
    return NULL;
}

// checks id is unique
int checDublicateID(int ID)
{
    if (totalProducts == 0)
    {
        return 1;
    }

    for (int currentIndex = 0; currentIndex < totalProducts; currentIndex++)
    {
        if (inventory[currentIndex].productID == ID)
        {
            return 0;
        }
    }
    return 1;
}

// add new product
void addProduct()
{
    int newID = 0, newQuantity = -1;
    float newPrice = -1;
    printf("Enter ID of new product: ");
    int inputCheck = scanf("%d", &newID);
    if (inputCheck != 1)
    {
        printf("Error. Enter integer");
        return;
    }
    else if (newID < 1 || newID > MAXID)
    {
        printf("Error. ID must be between 1 and %d", MAXID);
        return;
    }
    else if (!checDublicateID(newID))
    {
        printf("Error. Dublicate ID entered");
        return;
    }

    totalProducts++;
    product *temp = (product *)realloc(inventory, sizeof(product) * totalProducts);
    if (temp != NULL)
    {
        inventory = temp;
    }
    else
    {
        printf("Error. Failed to reallocate memory");
        totalProducts--;
        return;
    }
    inventory[totalProducts - 1].productID = newID;

    clearInput();
    printf("Enter name of product: ");
    inputCheck = scanf("%49[^\n]", inventory[totalProducts - 1].productName);
    if (inputCheck != 1)
    {
        printf("Error. Enter string");
        return;
    }

    printf("Enter Price: ");
    inputCheck = scanf("%f", &newPrice);
    if (inputCheck != 1)
    {
        printf("Error. Enter a number");
        return;
    }
    else if (newPrice < 0 || newPrice > MAXPRICE)
    {
        printf("Error. Price must be between 0 and %d", MAXPRICE);
        return;
    }
    inventory[totalProducts - 1].price = newPrice;

    printf("Enter Quantity: ");
    inputCheck = scanf("%d", &newQuantity);
    if (inputCheck != 1)
    {
        printf("Error. Enter a number");
        return;
    }
    else if (newQuantity < 0 || newQuantity > MAXQTY)
    {
        printf("Error. Quantity must be between 0 and %d", MAXQTY);
        return;
    }
    inventory[totalProducts - 1].quantity = newQuantity;
    printf("Product added successfully\n");
}

// display products
void displayProducts()
{
    if (totalProducts == 0)
    {
        printf("Inventory is empty");
        return;
    }
    printf("\n===== LIST OF PRODUCTS =====\n");
    for (int currentIndex = 0; currentIndex < totalProducts; currentIndex++)
    {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", inventory[currentIndex].productID, inventory[currentIndex].productName, inventory[currentIndex].price, inventory[currentIndex].quantity);
    }
}

// update quantity by id
void updateQuantity()
{
    if (totalProducts == 0)
    {
        printf("Inventory is empty.");
        return;
    }

    int ID = 0;
    printf("Enter ID of the product: ");
    int inputCheck = scanf("%d", &ID);
    if (inputCheck != 1)
    {
        printf("Error. Enter integer");
        return;
    }

    for (int currentIndex = 0; currentIndex < totalProducts; currentIndex++)
    {
        if (inventory[currentIndex].productID == ID)
        {
            int newQty = 0;
            printf("Enter updated quantity: ");
            inputCheck = scanf("%d", &newQty);
            if (inputCheck != 1)
            {
                printf("Error. Enter a number");
                return;
            }
            else if (newQty < 0 || newQty > MAXQTY)
            {
                printf("Error. Quantity must be between 1 and %d", MAXQTY);
                return;
            }
            inventory[currentIndex].quantity = newQty;
            printf("Quantity updated successfully");
            return;
        }
    }
    printf("No product with ID %d found", ID);
}

// search product details by id
void searchByID()
{
    if (totalProducts == 0)
    {
        printf("Inventory is empty.");
        return;
    }

    int ID = 0, found = 0;
    printf("Enter ID of the product: ");
    int inputCheck = scanf("%d", &ID);
    if (inputCheck != 1)
    {
        printf("Error. Enter integer");
        return;
    }

    for (int currentIndex = 0; currentIndex < totalProducts; currentIndex++)
    {
        if (inventory[currentIndex].productID == ID)
        {
            found = 1;
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", inventory[currentIndex].productID, inventory[currentIndex].productName, inventory[currentIndex].price, inventory[currentIndex].quantity);
        }
    }
    if (!found)
    {
        printf("No product with ID %d found", ID);
    }
}

// search product details by name
void searchByName()
{
    if (totalProducts == 0)
    {
        printf("Inventory is empty.");
        return;
    }

    char matchString[NAMELENGTH];
    int found = 0;
    clearInput();
    printf("Enter string to search (can be partial): ");
    scanf("%49[^\n]", matchString);
    for (int currentIndex = 0; currentIndex < totalProducts; currentIndex++)
    {
        if (searchString(inventory[currentIndex].productName, matchString) != NULL)
        {
            found = 1;
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", inventory[currentIndex].productID, inventory[currentIndex].productName, inventory[currentIndex].price, inventory[currentIndex].quantity);
        }
    }
    if (!found)
    {
        printf("No product with similar name found");
    }
}

// search product details by price range
void searchByPriceRange()
{
    if (totalProducts == 0)
    {
        printf("Inventory is empty.");
        return;
    }

    int found = 0, minPrice, maxPrice;
    printf("Enter minimum and maximum price (both inclusive): ");
    int inputCheck = scanf("%d %d", &minPrice, &maxPrice);
    if (inputCheck != 2)
    {
        printf("Error. Enter number");
        return;
    }

    for (int currentIndex = 0; currentIndex < totalProducts; currentIndex++)
    {
        if (inventory[currentIndex].price >= minPrice && inventory[currentIndex].price <= maxPrice)
        {
            found = 1;
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", inventory[currentIndex].productID, inventory[currentIndex].productName, inventory[currentIndex].price, inventory[currentIndex].quantity);
        }
    }

    if (!found)
    {
        printf("No product found in given range");
    }
}

// delete product by id
void deleteProduct()
{
    if (totalProducts == 0)
    {
        printf("Inventory is empty.");
        return;
    }

    int found = 0, ID;
    printf("Enter ID of product: ");
    int inputCheck = scanf("%d", &ID);
    if (inputCheck != 1)
    {
        printf("Error. Enter integer");
        return;
    }
    for (int currentIndex = 0; currentIndex < totalProducts; currentIndex++)
    {
        if (inventory[currentIndex].productID == ID)
        {
            found = 1;
            for (int nextIndex = currentIndex; nextIndex < totalProducts - 1; nextIndex++)
            {
                inventory[nextIndex] = inventory[nextIndex + 1];
            }
            totalProducts--;
            product *temp = (product *)realloc(inventory, sizeof(product) * totalProducts);
            if (totalProducts != 0 && temp == NULL)
            {
                printf("Error. Failed to reallocate memory");
                totalProducts++;
                return;
            }
            else
            {
                inventory = temp;
            }
            printf("Product deleted successfully");
            break;
        }
    }
    if (!found)
    {
        printf("No product with ID %d found", ID);
    }
}