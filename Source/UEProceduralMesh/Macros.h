#pragma once

#define DEBUG_MESSAGE(x, ...) if(GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(x), __VA_ARGS__)); }
