#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

int main(void)
{
    // You will need this when converting from grams to pounds (lbs)
    const double GRAMS_IN_LBS = 453.5924;

    // Declaration per group c = Coffee, c1 = Coffee 1, c2 = Coffee 2, c3 = Coffee 3 
    char c1Type, c1GrindSize, c1BestWithCream,
    c2Type, c2GrindSize, c2BestWithCream,
    c3Type, c3GrindSize, c3BestWithCream, 
    cStrength, cLikeWithCream, cMaker;

    int c1BagWeight,
    c2BagWeight,  
    c3BagWeight, 
    cNumberOfDailyServe;

    double c1ServingTemp, 
    c2ServingTemp, 
    c3ServingTemp;


	// Note:
	// You can convert Celsius to Fahrenheit given the following formula:
	// fahrenheit = (celsius * 1.8) + 32.0);

    printf("Take a Break - Coffee Shop\n");
    printf("==========================\n\n");

    printf("Enter the coffee product information being sold today...\n\n");

    printf("COFFEE-1...\n");
    printf("Type ([L]ight,[B]lend): ");
    scanf(" %c", &c1Type);
    printf("Grind size ([C]ourse,[F]ine): ");
    scanf(" %c", &c1GrindSize);
    printf("Bag weight (g): ");
    scanf("%d", &c1BagWeight);
    printf("Best served with cream ([Y]es,[N]o): ");
    scanf(" %c", &c1BestWithCream);
    printf("Ideal serving temperature (Celsius): ");
    scanf("%lf", &c1ServingTemp);

    printf("\nCOFFEE-2...\n");
    printf("Type ([L]ight,[B]lend): ");
    scanf(" %c", &c2Type);
    printf("Grind size ([C]ourse,[F]ine): ");
    scanf(" %c", &c2GrindSize);
    printf("Bag weight (g): ");
    scanf("%d", &c2BagWeight);
    printf("Best served with cream ([Y]es,[N]o): ");
    scanf(" %c", &c2BestWithCream);
    printf("Ideal serving temperature (Celsius): ");
    scanf("%lf", &c2ServingTemp);

    printf("\nCOFFEE-3...\n");
    printf("Type ([L]ight,[B]lend): ");
    scanf(" %c", &c3Type);
    printf("Grind size ([C]ourse,[F]ine): ");
    scanf(" %c", &c3GrindSize);
    printf("Bag weight (g): ");
    scanf("%d", &c3BagWeight);
    printf("Best served with cream ([Y]es,[N]o): ");
    scanf(" %c", &c3BestWithCream);
    printf("Ideal serving temperature (Celsius): ");
    scanf("%lf", &c3ServingTemp);

    printf("\n---+---------------+---------------+---------------+-------+--------------\n");
    printf("   |    Coffee     |    Coffee     |   Packaged    | Best  |   Serving\n");
    printf("   |     Type      |  Grind Size   |  Bag Weight   | Served| Temperature\n");
    printf("   +---------------+---------------+---------------+ With  +--------------\n");
    printf("ID | Light | Blend | Course | Fine |  (G) | Lbs    | Cream |   (C) |   (F)\n");
    printf("---+---------------+---------------+---------------+-------+--------------\n");

    printf(" 1 |   %d   |   %d   |    %d   |   %d  | %4d | %6.3lf |   %d   | %5.1lf | %5.1lf\n", 
    (c1Type == 'l' || c1Type == 'L') ? 1 : 0, 
    (c1Type == 'b' || c1Type == 'B') ? 1 : 0,
    (c1GrindSize == 'c' || c1GrindSize == 'C') ? 1 : 0,
    (c1GrindSize == 'f' || c1GrindSize == 'F') ? 1 : 0,
    c1BagWeight, 
    (c1BagWeight / GRAMS_IN_LBS), 
    (c1BestWithCream == 'y' || c1BestWithCream == 'Y') ? 1 : 0, 
    c1ServingTemp, 
    (c1ServingTemp * 1.8) + 32.0);

    printf(" 2 |   %d   |   %d   |    %d   |   %d  | %4d | %6.3lf |   %d   | %5.1lf | %5.1lf\n", 
    (c2Type == 'l' || c2Type == 'L') ? 1 : 0, 
    (c2Type == 'b' || c2Type == 'B') ? 1 : 0,
    (c2GrindSize == 'c' || c2GrindSize == 'C') ? 1 : 0,
    (c2GrindSize == 'f' || c2GrindSize == 'F') ? 1 : 0,
    c2BagWeight, 
    (c2BagWeight / GRAMS_IN_LBS), 
    (c2BestWithCream == 'y' || c2BestWithCream == 'Y') ? 1 : 0, 
    c2ServingTemp, 
    (c2ServingTemp * 1.8) + 32.0);
    
    printf(" 3 |   %d   |   %d   |    %d   |   %d  | %4d | %6.3lf |   %d   | %5.1lf | %5.1lf\n\n", 
    (c3Type == 'l' || c3Type == 'L') ? 1 : 0, 
    (c3Type == 'b' || c3Type == 'B') ? 1 : 0,
    (c3GrindSize == 'c' || c3GrindSize == 'C') ? 1 : 0,
    (c3GrindSize == 'f' || c3GrindSize == 'F') ? 1 : 0,
    c3BagWeight, 
    (c3BagWeight / GRAMS_IN_LBS), 
    (c3BestWithCream == 'y' || c3BestWithCream == 'Y') ? 1 : 0, 
    c3ServingTemp, 
    (c3ServingTemp * 1.8) + 32.0);

    printf("Enter how you like your coffee and the coffee maker equipment you use...\n\n");

    printf("Coffee strength ([M]ild,[R]ich): ");
    scanf(" %c", &cStrength);
    printf("Do you like your coffee with cream ([Y]es,[N]o): ");
    scanf(" %c", &cLikeWithCream);
    printf("Typical number of daily servings: ");
    scanf("%d", &cNumberOfDailyServe);
    printf("Maker ([R]esidential,[C]ommercial): ");
    scanf(" %c", &cMaker);

    printf("\nThe below table shows how your preferences align to the available products:\n\n");

    printf("--------------------+--------------------+-------------+-------+--------------\n");
    printf("  |     Coffee      |       Coffee       |  Packaged   | With  |   Serving\n");
    printf("ID|      Type       |     Grind Size     | Bag Weight  | Cream | Temperature\n");
    printf("--+-----------------+--------------------+-------------+-------+--------------\n");

    printf(" 1|       %d         |         %d          |      %d      |   %d   |      %d\n", 
    (c1Type == 'l' || c1Type == 'L') ? (cStrength == 'm' || cStrength == 'M') ? 1 : 0 : (cStrength == 'r' || cStrength == 'R') ? 1 : 0, 
    (c1GrindSize == 'c' || c1GrindSize == 'C') ? (cMaker == 'r' || cMaker == 'R') ? 1 : 0 : (cMaker == 'c' || cMaker == 'C') ? 1 : 0,
    (c1BagWeight >= 0 && c1BagWeight <= 250) ? (cNumberOfDailyServe >= 1 && cNumberOfDailyServe <= 4) ? 1 : 0 : 
    (c1BagWeight > 250 && c1BagWeight <= 999) ? (cNumberOfDailyServe >= 5 && cNumberOfDailyServe <= 9) ? 1 : 0 : 
    (cNumberOfDailyServe >= 10) ? 1 : 0, 
    (c1BestWithCream == 'y' || c1BestWithCream == 'Y') ? (cLikeWithCream == 'y' || cLikeWithCream == 'Y') ? 1 : 0 : 
    (cLikeWithCream == 'n' || cLikeWithCream == 'N') ? 1 : 0, 
    (cMaker == 'r' || cMaker == 'R') ? (c1ServingTemp >= 60.0 && c1ServingTemp <= 69.9) ? 1 : 0 : (c1ServingTemp >= 70.0) ? 1 : 0);

    printf(" 2|       %d         |         %d          |      %d      |   %d   |      %d\n", 
    (c2Type == 'l' || c2Type == 'L') ? (cStrength == 'm' || cStrength == 'M') ? 1 : 0 : (cStrength == 'r' || cStrength == 'R') ? 1 : 0, 
    (c2GrindSize == 'c' || c2GrindSize == 'C') ? (cMaker == 'r' || cMaker == 'R') ? 1 : 0 : (cMaker == 'c' || cMaker == 'C') ? 1 : 0,
    (c2BagWeight >= 0 && c2BagWeight <= 250) ? (cNumberOfDailyServe >= 1 && cNumberOfDailyServe <= 4) ? 1 : 0 : 
    (c2BagWeight > 250 && c2BagWeight <= 999) ? (cNumberOfDailyServe >= 5 && cNumberOfDailyServe <= 9) ? 1 : 0 : 
    (cNumberOfDailyServe >= 10) ? 1 : 0, 
    (c2BestWithCream == 'y' || c2BestWithCream == 'Y') ? (cLikeWithCream == 'y' || cLikeWithCream == 'Y') ? 1 : 0 : 
    (cLikeWithCream == 'n' || cLikeWithCream == 'N') ? 1 : 0, 
    (cMaker == 'r' || cMaker == 'R') ? (c2ServingTemp >= 60.0 && c2ServingTemp <= 69.9) ? 1 : 0 : (c2ServingTemp >= 70.0) ? 1 : 0);

    printf(" 3|       %d         |         %d          |      %d      |   %d   |      %d\n\n", 
    (c3Type == 'l' || c3Type == 'L') ? (cStrength == 'm' || cStrength == 'M') ? 1 : 0 : (cStrength == 'r' || cStrength == 'R') ? 1 : 0, 
    (c3GrindSize == 'c' || c3GrindSize == 'C') ? (cMaker == 'r' || cMaker == 'R') ? 1 : 0 : (cMaker == 'c' || cMaker == 'C') ? 1 : 0,
    (c3BagWeight >= 0 && c3BagWeight <= 250) ? (cNumberOfDailyServe >= 1 && cNumberOfDailyServe <= 4) ? 1 : 0 : 
    (c3BagWeight > 250 && c3BagWeight <= 999) ? (cNumberOfDailyServe >= 5 && cNumberOfDailyServe <= 9) ? 1 : 0 : 
    (cNumberOfDailyServe >= 10) ? 1 : 0, 
    (c3BestWithCream == 'y' || c3BestWithCream == 'Y') ? (cLikeWithCream == 'y' || cLikeWithCream == 'Y') ? 1 : 0 : 
    (cLikeWithCream == 'n' || cLikeWithCream == 'N') ? 1 : 0, 
    (cMaker == 'r' || cMaker == 'R') ? (c3ServingTemp >= 60.0 && c3ServingTemp <= 69.9) ? 1 : 0 : (c3ServingTemp >= 70.0) ? 1 : 0);

    printf("Enter how you like your coffee and the coffee maker equipment you use...\n\n");

    printf("Coffee strength ([M]ild,[R]ich): ");
    scanf(" %c", &cStrength);
    printf("Do you like your coffee with cream ([Y]es,[N]o): ");
    scanf(" %c", &cLikeWithCream);
    printf("Typical number of daily servings: ");
    scanf("%d", &cNumberOfDailyServe);
    printf("Maker ([R]esidential,[C]ommercial): ");
    scanf(" %c", &cMaker);

    printf("\nThe below table shows how your preferences align to the available products:\n\n");

    printf("--------------------+--------------------+-------------+-------+--------------\n");
    printf("  |     Coffee      |       Coffee       |  Packaged   | With  |   Serving\n");
    printf("ID|      Type       |     Grind Size     | Bag Weight  | Cream | Temperature\n");
    printf("--+-----------------+--------------------+-------------+-------+--------------\n");

    printf(" 1|       %d         |         %d          |      %d      |   %d   |      %d\n", 
    (c1Type == 'l' || c1Type == 'L') ? (cStrength == 'm' || cStrength == 'M') ? 1 : 0 : (cStrength == 'r' || cStrength == 'R') ? 1 : 0, 
    (c1GrindSize == 'c' || c1GrindSize == 'C') ? (cMaker == 'r' || cMaker == 'R') ? 1 : 0 : (cMaker == 'c' || cMaker == 'C') ? 1 : 0,
    (c1BagWeight >= 0 && c1BagWeight <= 250) ? (cNumberOfDailyServe >= 1 && cNumberOfDailyServe <= 4) ? 1 : 0 : 
    (c1BagWeight > 250 && c1BagWeight <= 999) ? (cNumberOfDailyServe >= 5 && cNumberOfDailyServe <= 9) ? 1 : 0 : 
    (cNumberOfDailyServe >= 10) ? 1 : 0, 
    (c1BestWithCream == 'y' || c1BestWithCream == 'Y') ? (cLikeWithCream == 'y' || cLikeWithCream == 'Y') ? 1 : 0 : 
    (cLikeWithCream == 'n' || cLikeWithCream == 'N') ? 1 : 0, 
    (cMaker == 'r' || cMaker == 'R') ? (c1ServingTemp >= 60.0 && c1ServingTemp <= 69.9) ? 1 : 0 : (c1ServingTemp >= 70.0) ? 1 : 0);

    printf(" 2|       %d         |         %d          |      %d      |   %d   |      %d\n", 
    (c2Type == 'l' || c2Type == 'L') ? (cStrength == 'm' || cStrength == 'M') ? 1 : 0 : (cStrength == 'r' || cStrength == 'R') ? 1 : 0, 
    (c2GrindSize == 'c' || c2GrindSize == 'C') ? (cMaker == 'r' || cMaker == 'R') ? 1 : 0 : (cMaker == 'c' || cMaker == 'C') ? 1 : 0,
    (c2BagWeight >= 0 && c2BagWeight <= 250) ? (cNumberOfDailyServe >= 1 && cNumberOfDailyServe <= 4) ? 1 : 0 : 
    (c2BagWeight > 250 && c2BagWeight <= 999) ? (cNumberOfDailyServe >= 5 && cNumberOfDailyServe <= 9) ? 1 : 0 : 
    (cNumberOfDailyServe >= 10) ? 1 : 0, 
    (c2BestWithCream == 'y' || c2BestWithCream == 'Y') ? (cLikeWithCream == 'y' || cLikeWithCream == 'Y') ? 1 : 0 : 
    (cLikeWithCream == 'n' || cLikeWithCream == 'N') ? 1 : 0, 
    (cMaker == 'r' || cMaker == 'R') ? (c2ServingTemp >= 60.0 && c2ServingTemp <= 69.9) ? 1 : 0 : (c2ServingTemp >= 70.0) ? 1 : 0);

    printf(" 3|       %d         |         %d          |      %d      |   %d   |      %d\n\n", 
    (c3Type == 'l' || c3Type == 'L') ? (cStrength == 'm' || cStrength == 'M') ? 1 : 0 : (cStrength == 'r' || cStrength == 'R') ? 1 : 0, 
    (c3GrindSize == 'c' || c3GrindSize == 'C') ? (cMaker == 'r' || cMaker == 'R') ? 1 : 0 : (cMaker == 'c' || cMaker == 'C') ? 1 : 0,
    (c3BagWeight >= 0 && c3BagWeight <= 250) ? (cNumberOfDailyServe >= 1 && cNumberOfDailyServe <= 4) ? 1 : 0 : 
    (c3BagWeight > 250 && c3BagWeight <= 999) ? (cNumberOfDailyServe >= 5 && cNumberOfDailyServe <= 9) ? 1 : 0 : 
    (cNumberOfDailyServe >= 10) ? 1 : 0, 
    (c3BestWithCream == 'y' || c3BestWithCream == 'Y') ? (cLikeWithCream == 'y' || cLikeWithCream == 'Y') ? 1 : 0 : 
    (cLikeWithCream == 'n' || cLikeWithCream == 'N') ? 1 : 0, 
    (cMaker == 'r' || cMaker == 'R') ? (c3ServingTemp >= 60.0 && c3ServingTemp <= 69.9) ? 1 : 0 : (c3ServingTemp >= 70.0) ? 1 : 0);

    printf("Hope you found a product that suits your likes!\n\n");

    return 0;
}


/*

Provided formatting parts for printf statements:

As described in step-7
======================
printf(" 1 |   %d   |   %d   |    %d   |   %d  | %4d | %6.3lf |   %d   | %5.1lf | %5.1lf\n",

As described in step-10
=======================
printf(" 1|       %d         |         %d          |      %d      |   %d   |      %d\n",

*/