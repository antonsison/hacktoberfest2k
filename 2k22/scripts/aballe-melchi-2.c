#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void)
{
    const double TAX = 0.13;
    const char patSize = 'S';
    const char tomSize = 'L';
    const char salSize = 'M';

    double smallPrice = 0, mediumPrice = 0, largePrice = 0;
    int subTotalSmall = 0, totalTaxSmall = 0, totalAmountSmall = 0, 
    subTotalMedium = 0, totalTaxMedium = 0, totalAmountMedium = 0,
    subTotalLarge = 0, totalTaxLarge = 0, totalAmountLarge = 0,
    subTotal = 0, totalTax = 0, totalAmount = 0,
    numOfShirtToPurchaseSmall = 0, numOfShirtToPurchaseMedium = 0, numOfShirtToPurchaseLarge = 0, toonies = 0, 
    loonies = 0, quarters = 0, dimes = 0, nickels = 0, pennies = 0, remainder = 0, wholeNum = 0;

    printf("Set Shirt Prices\n");
    printf("================\n");

    printf("Enter the price for a SMALL shirt: $");
    scanf("%lf", &smallPrice);
    printf("Enter the price for a MEDIUM shirt: $");
    scanf("%lf", &mediumPrice);
    printf("Enter the price for a LARGE shirt: $");
    scanf("%lf", &largePrice);

    printf("\nShirt Store Price List\n");
    printf("======================\n");
    printf("SMALL  : $%.2lf\n", smallPrice);
    printf("MEDIUM : $%.2lf\n", mediumPrice);
    printf("LARGE  : $%.2lf\n", largePrice);

    printf("\nPatty's shirt size is \'%c\'\n", patSize);
    printf("Number of shirts Patty is buying: ");
    scanf("%d", &numOfShirtToPurchaseSmall);

    printf("\nTommy's shirt size is \'%c\'\n", tomSize);
    printf("Number of shirts Tommy is buying: ");
    scanf("%d", &numOfShirtToPurchaseLarge);

    printf("\nSally's shirt size is \'%c\'\n", salSize);
    printf("Number of shirts Sally is buying: ");
    scanf("%d", &numOfShirtToPurchaseMedium);

    subTotalSmall = (int)((smallPrice * numOfShirtToPurchaseSmall + .005) * 100);
    totalTaxSmall = (int)((((double)subTotalSmall / 100) * TAX + .005) * 100);
    totalAmountSmall = subTotalSmall + totalTaxSmall;

    subTotalMedium = (int)((mediumPrice * numOfShirtToPurchaseMedium + .005) * 100);
    totalTaxMedium = (int)((((double)subTotalMedium / 100) * TAX + .005) * 100);
    totalAmountMedium = subTotalMedium + totalTaxMedium;

    subTotalLarge = (int)((largePrice * numOfShirtToPurchaseLarge + .005) * 100);
    totalTaxLarge = (int)((((double)subTotalLarge / 100) * TAX + .005) * 100);
    totalAmountLarge = subTotalLarge + totalTaxLarge;

    subTotal = (int)((((double)subTotalSmall / 100) + ((double)subTotalMedium / 100) + ((double)subTotalLarge / 100) + .005) * 100);
    totalTax = (int)((((double)totalTaxSmall / 100) + ((double)totalTaxMedium / 100) + ((double)totalTaxLarge / 100) + .005) * 100);
    totalAmount = subTotal + totalTax;

    printf("\nCustomer Size Price Qty Sub-Total       Tax     Total\n");
    printf("-------- ---- ----- --- --------- --------- ---------\n");
    printf("Patty    %-4c %5.2lf %3d %9.4lf %9.4lf %9.4lf\n", 
    patSize, smallPrice, numOfShirtToPurchaseSmall, (double)subTotalSmall / 100, (double)totalTaxSmall / 100, (double)totalAmountSmall / 100);
    printf("Sally    %-4c %5.2lf %3d %9.4lf %9.4lf %9.4lf\n", 
    salSize, mediumPrice, numOfShirtToPurchaseMedium, (double)subTotalMedium / 100, (double)totalTaxMedium / 100, (double)totalAmountMedium / 100);
    printf("Tommy    %-4c %5.2lf %3d %9.4lf %9.4lf %9.4lf\n", 
    tomSize, largePrice, numOfShirtToPurchaseLarge, (double)subTotalLarge / 100, (double)totalTaxLarge / 100, (double)totalAmountLarge / 100);  
    printf("-------- ---- ----- --- --------- --------- ---------\n");
    printf("%33.4lf %9.4lf %9.4lf\n\n", (double)subTotal / 100, (double)totalTax / 100, (double)totalAmount / 100);

    printf("Daily retail sales represented by coins\n");
    printf("=======================================\n\n");

    printf("Sales EXCLUDING tax\n");
    printf("Coin     Qty   Balance\n");
    printf("-------- --- ---------\n");
    printf("%22.4lf\n", (double)subTotal / 100);

    toonies = (int)(((double)subTotal / 100) / 2);
    wholeNum = (double)subTotal / 100;
    remainder = ((((double)(subTotal % 2) + ((((double)subTotal / 100) - wholeNum))) + .005) * 100);

    printf("Toonies  %3d %9.4lf\n", toonies, (double)remainder / 100);
    loonies = (((double)remainder /100) / 1);
    remainder = ((((double)remainder / 100) - loonies) + 0.005) * 100;
    printf("Loonies  %3d %9.4lf\n", loonies, (double)remainder / 100);
    quarters = (((double)remainder /100) / .25);
    remainder = ((((double)remainder / 100) - (quarters * .25)) + 0.005) * 100;
    printf("Quarters %3d %9.4lf\n", quarters, (double)remainder / 100);
    dimes = (((double)remainder /100) / .10);
    remainder = ((((double)remainder / 100) - (dimes * .10)) + 0.005) * 100;
    printf("Dimes    %3d %9.4lf\n", dimes, (double)remainder / 100);
    nickels = (((double)remainder /100) / .05);
    remainder = ((((double)remainder / 100) - (nickels * .05)) + 0.005) * 100;
    printf("Nickels  %3d %9.4lf\n", nickels, (double)remainder / 100);
    pennies = (((double)remainder / 100) / .01);
    remainder = ((((double)remainder / 100) - (pennies * .01)) + 0.005) * 100;
    printf("Pennies  %3d %9.4lf\n\n", pennies, (double)remainder / 100);

    printf("Average cost/shirt: $%.4lf\n\n", (double)subTotal / 100 / (numOfShirtToPurchaseSmall + 
    numOfShirtToPurchaseMedium + numOfShirtToPurchaseLarge));
    
    printf("Sales INCLUDING tax\n");
    printf("Coin     Qty   Balance\n");
    printf("-------- --- ---------\n");
    printf("%22.4lf\n", (double)totalAmount / 100);
    
    toonies = (int)(((double)totalAmount / 100) / 2);
    wholeNum = (double)totalAmount / 100;
    remainder = ((((double)(totalAmount % 2) + ((((double)totalAmount / 100) - wholeNum))) + .005) * 100);

    printf("Toonies  %3d %9.4lf\n", toonies, (double)remainder / 100);
    loonies = (((double)remainder /100) / 1);
    remainder = ((((double)remainder / 100) - loonies) + 0.005) * 100;
    printf("Loonies  %3d %9.4lf\n", loonies, (double)remainder / 100);
    quarters = (((double)remainder /100) / .25);
    remainder = ((((double)remainder / 100) - (quarters * .25)) + 0.005) * 100;
    printf("Quarters %3d %9.4lf\n", quarters, (double)remainder / 100);
    dimes = (((double)remainder /100) / .10);
    remainder = ((((double)remainder / 100) - (dimes * .10)) + 0.005) * 100;
    printf("Dimes    %3d %9.4lf\n", dimes, (double)remainder / 100);
    nickels = (((double)remainder /100) / .05);
    remainder = ((((double)remainder / 100) - (nickels * .05)) + 0.005) * 100;
    printf("Nickels  %3d %9.4lf\n", nickels, (double)remainder / 100);
    pennies = (((double)remainder / 100) / .01);
    remainder = ((((double)remainder / 100) - (pennies * .01)) + 0.005) * 100;
    printf("Pennies  %3d %9.4lf\n\n", pennies, (double)remainder / 100);

    printf("Average cost/shirt: $%.4lf\n\n", ((double)totalAmount / 100) / (numOfShirtToPurchaseSmall + 
    numOfShirtToPurchaseMedium + numOfShirtToPurchaseLarge));
    return 0;
}