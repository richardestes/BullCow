// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    
    int32 Number = 9;
    
    Isograms = GetValidWords(Words);
                
    SetupGame(); 
    
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{        
    if (bGameOver) 
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame()
{
    // Greet Player
    PrintLine(TEXT("Welcome to Bull Cows!"));
        
    HiddenWord = Isograms[FMath::RandRange(0,Isograms.Num() - 1)];
    Lives = HiddenWord.Len();
    bGameOver = false;
    
    PrintLine(TEXT("Try to guess the %i letter word!"), HiddenWord.Len());
    // PrintLine(TEXT("The HiddenWord is: %s."), *HiddenWord); //DEBUG
    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("If you get 3 Bulls or 5 Cows,\nyou will gain a life."));
    PrintLine(TEXT("Type in your guess and \npress enter to continue..."));         
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (HiddenWord.Len() != Guess.Len())
    {
        PrintLine(TEXT("The hidden word is %i letters long"), HiddenWord.Len());    
        PrintLine(TEXT("Sorry, try guessing again! \nYou have %i lives remaining."), Lives);
        return;
    }
    
    if (Guess == HiddenWord) 
    {
        PrintLine(TEXT("You have won!"));
        EndGame();
        return;
    }
    
    // Check if Isogram
    if (!IsIsogram(Guess))
    {
        // code
        PrintLine(TEXT("No repeating letters, guess again!"));
        return;
    }
    
    // Remove Life
    PrintLine(TEXT("Lost a life!")); 
    --Lives;
    
    // Check if Lives > 0
    if (Lives <= 0) 
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();
        return;
    }
    
    // Show the player Bulls and Cows
    FBullCowCount Score = GetBullCows(Guess);
    
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls,Score.Cows);
    
    if (Score.Bulls == 3)
    {
        Lives++;
        PrintLine(TEXT("Life Up!"));
    }
    
    if (Score.Cows == 5)
    {
        Lives++;
        PrintLine("Life Up!");
    }
    
    PrintLine(TEXT("Guess again, you have %i lives left."), Lives);
    
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{    
    for (int32 Index = 0; Index < Word.Len(); Index++) // Check for any matching characters
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
            
        }
        
    }
    
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;
    
    for (FString Word:WordList)
    {
        if (Word.Len() >=4 && Word.Len() <=8) // Check word length
        {
            if (IsIsogram(Word)) // Check if word is isogram
            {
                ValidWords.Emplace(Word);
            }
        } 
        
    }
    
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;
    
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue; // starts a new iteration of the current for loop
        }
        
        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break; // exits current loop and starts new iteration of OUTER for loop
            }
        }
    }
    return Count;
}