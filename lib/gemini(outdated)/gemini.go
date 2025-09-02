package main

import (
	"C"
	"context"
	"fmt"
	"log"
	"os"

	"google.golang.org/genai"
)

//export Translate
func Translate(Input string, client *genai.Client, ctx context.Context) string {
	result, err := client.Models.GenerateContent(
		ctx,
		"gemini-2.0-flash",
		genai.Text(Input),
		nil,
	)

	if err != nil {
		log.Fatal(err)
	}

	return result.Text()
}

func main() {

	ctx := context.Background()
	// Access your API key as an environment variable (see "Set up your API key" below)
	apiKey := os.Getenv("GEMINI_API_KEY")
	if apiKey == "" {
		log.Fatal("GEMINI_API_KEY environment variable not set")
	}

	client, err := genai.NewClient(ctx, &genai.ClientConfig{
		APIKey:  apiKey,
		Backend: genai.BackendGeminiAPI,
	})

	if err != nil {
		log.Fatal(err)
	}

	testString := "tell me about AI in 5 words"

	result := Translate(testString, client, ctx)
	fmt.Println(result)
}
