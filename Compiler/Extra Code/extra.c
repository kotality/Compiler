// Read input file and print contents
// void printParserIn(int flag)
// {
//     int input;

//     while (fscanf(inFile, "%d", &input) != EOF)
//     {
//         tokenArray[tokenVal] = input;
//         tokenVal++;

//         // Check if there's an identifier
//         if (input == identsym)
//         {
//             fscanf(inFile, "%d", &input);
//             tokenArray[tokenVal] = input;
//             tokenVal++;

//             if (flag == 1)
//                 printf("identym %d ", input);
//             fprintf(outFile, "identym %d ", input);
//         }
//         // Get number
//         else if (input == numbersym)
//         {
//             fscanf(inFile, "%d", &input);
//             tokenArray[tokenVal] = input;
//             tokenVal++;

//             if (flag == 1)
//                 printf("numbersym %d ", input);
//             fprintf(outFile, "numbersym %d ", input);
//         }
//         printTokens(input, flag);
//     }
// }