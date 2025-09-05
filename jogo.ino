#include <Arduino.h>
#include <Keypad.h>
#include <avr/pgmspace.h>

// Variaveis das notas
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define REST 0

// porta do buzzer
int BUZZER_PIN = 11;

//logica do jogo A
const int BOTAO_PERDER_VIDA_PIN = 10;
bool botaoPressionadoAnteriormente = false;
enum GameState {
  STANDBY,
  TOCANDO_MUSICA,
  ESPERANDO_RESPOSTA,
  FIM_DE_JOGO,
  TOCANDO_MUSICA_EXTRA
};

GameState estadoAtual = STANDBY;
int rodadaAtual = 0;
int respostasNao = 0;
int musicIndex;
bool esperandoNumeroDaMusica = false;


//musicas
// Melody 1
const int16_t melody_1[] PROGMEM = {
  NOTE_AS4, 8, NOTE_AS4, 8, NOTE_AS4, 8,
  NOTE_F5, 2, NOTE_C6, 2,
  NOTE_AS5, 8, NOTE_A5, 8, NOTE_G5, 8, NOTE_F6, 2, NOTE_C6, 4,
  NOTE_AS5, 8, NOTE_A5, 8, NOTE_G5, 8, NOTE_F6, 2, NOTE_C6, 4
};

// Melody 2
const int melody_2[] PROGMEM = {
  NOTE_D4, 4, NOTE_G4, 4, NOTE_AS4, 8, NOTE_A4, 4, NOTE_G4, 2, NOTE_D5, 2,
  NOTE_CS5, 4, NOTE_C5, 8, NOTE_A4, 4, NOTE_G4, 2, NOTE_D4, 2,
  NOTE_G4, 4, NOTE_AS4, 8, NOTE_A4, 4, NOTE_G4, 2
};


// Melody 3
const int melody_3[] PROGMEM = {
  NOTE_E4, 4, NOTE_F4, 4, NOTE_G4, 2,
  NOTE_C5, 4, NOTE_E5, 4, NOTE_G5, 2,
  NOTE_F5, 4, NOTE_E5, 4, NOTE_D5, 4, NOTE_C5, 4
};


// Melody 4
const int melody_4[] PROGMEM = {
  NOTE_E4, 8, NOTE_F4, 8, NOTE_FS4, 8, NOTE_G4, 8, NOTE_AS4, 4, NOTE_G4, 4,
  NOTE_E4, 8, NOTE_F4, 8, NOTE_FS4, 8, NOTE_G4, 8, NOTE_AS4, 4, NOTE_G4, 4
};


// Melody 5
const int melody_5[] PROGMEM = {
  NOTE_C5, 8, NOTE_C5, 8, NOTE_CS5, 8, NOTE_CS5, 8,
  NOTE_E5, 8, NOTE_F5, 8, NOTE_E5, 8, NOTE_CS5, 8,
  NOTE_AS4, 8, NOTE_C5, 8, NOTE_AS4, 8, NOTE_C5, 8
};


// Melody 6
const int melody_6[] PROGMEM = {
  NOTE_E4, 4, NOTE_F4, 4, NOTE_G4, 2,
  NOTE_D4, 4, NOTE_E4, 4, NOTE_F4, 2,
  NOTE_B3, 4, NOTE_C4, 4, NOTE_D4, 2
};


// Melody 7
const int melody_7[] PROGMEM = {
  NOTE_E4, 8, NOTE_G4, 8, NOTE_A4, 4, NOTE_E4, 8, NOTE_G4, 8, NOTE_B4, 4,
  NOTE_C5, 8, NOTE_D5, 8, NOTE_E5, 4
};


// Melody 8
const int melody_8[] PROGMEM = {
  NOTE_E4, 4, NOTE_G4, 4, NOTE_B4, 2,
  NOTE_A4, 4, NOTE_G4, 4, NOTE_F4, 2,
  NOTE_E4, 4, NOTE_D4, 4, NOTE_C4, 2
};


// Melody 9
const int melody_9[] PROGMEM = {
  NOTE_G4, 4, NOTE_C4, 4, NOTE_DS4, 2,
  NOTE_F4, 4, NOTE_G4, 4, NOTE_AS4, 2,
  NOTE_C5, 4, NOTE_DS4, 4, NOTE_D4, 2
};



// Melody: Som de Vitoria do Mario Bros
const int16_t melody_victory[] PROGMEM = {
  NOTE_C5, -4, NOTE_G5, 8, NOTE_E5, 4,
  NOTE_A5, 4, NOTE_B5, 4, NOTE_A5, 8, NOTE_GS5, 8, NOTE_AS5, 8,
  NOTE_GS5, 8, NOTE_A5, 4, REST, 8
};

// Melody: Som de Fim de Jogo do Mario Bros
const int16_t melody_game_over[] PROGMEM = {
  NOTE_C5,  -4,   NOTE_G4,  -4,NOTE_E4,   4, NOTE_A4, 
 -8,  NOTE_B4,  -8,  NOTE_A4,  -8,  NOTE_GS4,  -8,  NOTE_AS4,
  -8, NOTE_GS4,  -8,  NOTE_G4,  8,  NOTE_D4,  8, NOTE_E4,-2,
};

// Melody: 3 Bips
const int16_t melody_bips[] PROGMEM = {
  NOTE_C5,  8,   REST,  8,  NOTE_C5,  8,  REST,  8,  NOTE_C5,  8,  REST,  8,
};


// ARRAYS PARA SELEÇÃO ALEATÓRIA
const int16_t* randomMelodies[] = {
  melody_1,          // ID 0
  melody_2,          // ID 1
  melody_3,          // ID 2
  melody_4,          // ID 3
  melody_5,          // ID 4
  melody_6,          // ID 5
  melody_7,          // ID 6
  melody_8,          // ID 7
  melody_9,          // ID 8
  melody_victory,    // ID 10
  melody_game_over,  // ID 11
  melody_bips        // ID 12
};


const size_t randomMelodiesSize[] = {
  sizeof(melody_1) / sizeof(melody_1[0]), 
  sizeof(melody_2) / sizeof(melody_2[0]), 
  sizeof(melody_3) / sizeof(melody_3[0]), 
  sizeof(melody_4) / sizeof(melody_4[0]), 
  sizeof(melody_5) / sizeof(melody_5[0]), 
  sizeof(melody_6) / sizeof(melody_6[0]), 
  sizeof(melody_7) / sizeof(melody_7[0]),  
  sizeof(melody_8) / sizeof(melody_8[0]),  
  sizeof(melody_9) / sizeof(melody_9[0]),  
  sizeof(melody_victory) / sizeof(melody_victory[0]),
  sizeof(melody_game_over) / sizeof(melody_game_over[0]),
  sizeof(melody_bips) / sizeof(melody_bips[0])
};

const int randomMelodiesTempo[] = {
  120,  // Tempo para melody_1 
  180,  // Tempo para melody_2 
  140,  // Tempo para melody_3 
  130,  // Tempo para melody_4 
  140,  // Tempo para melody_5 
  90,   // Tempo para melody_6 
  130,  // Tempo para melody_7 
  100,  // Tempo para melody_8 
  110,  // Tempo para melody_9 
  180,  // Tempo para melody_victory
  100,  // Tempo para melody_game_over
  250   // Tempo para melody_bips
};

// Numero de musicas para o botão B
const int NUM_MELODIES = 10;

// Teclado
const byte ROWS = 4, COLS = 4;

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 2, 3, 4, 5 };
byte colPins[COLS] = { 6, 7, 8, 9 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

char entrada[8];
byte lenEntrada = 0;


// Logica da entrada no teclado e rodadas
void resetEntrada();
void play(const int16_t* melody, size_t melody_len, int tempo);
void proximaRodada(); 

// DECLARAÇÃO DE FUNÇÕES
void resetEntrada() {
  lenEntrada = 0;
  entrada[0] = '\0';
}

void play(const int16_t* melody, size_t melody_len, int tempo);


//FUNÇÕES PRINCIPAIS
void setup() {
  Serial.begin(9600);
  Serial.println("Debug: ");
  Serial.println("MENU: ");
  Serial.println("A - Jogo de musicas");
  Serial.println("B - Escolha uma musica");
  Serial.println("C - Musica aleatoria");

  resetEntrada();
}

// Funcao para avancar para a proxima rodada
void proximaRodada() {
  rodadaAtual++;
  if (rodadaAtual > 5) {
    estadoAtual = FIM_DE_JOGO;
    Serial.println("Parabens! Voce completou todas as 5 rodadas.");
    // Corrigido para melody_victory
    play(melody_victory, sizeof(melody_victory) / sizeof(melody_victory[0]), 180);
    Serial.println("FIM DE JOGO!");
    resetEntrada();
  } else {
    Serial.print("Rodada ");
    Serial.println(rodadaAtual);
    Serial.print("Vidas perdidas: ");
    Serial.print(respostasNao);
    Serial.println(".");
    musicIndex = random(5);
    play(randomMelodies[musicIndex], randomMelodiesSize[musicIndex], randomMelodiesTempo[musicIndex]);
    Serial.print("Tocando a melodia ID: ");
    Serial.println(musicIndex + 1);
    estadoAtual = ESPERANDO_RESPOSTA;
    Serial.println("Musica finalizada.");
    play(melody_bips, sizeof(melody_bips) / sizeof(melody_bips[0]), 180);
    Serial.println("Você conhece a musica?");
    Serial.println("Pressione '*' para SIM ou '#' para NAO.");
    resetEntrada();
  }
}

void loop() {

  char k = keypad.getKey();
  if (k) {
    Serial.print("Tecla: ");
    Serial.println(k);
  }



  if (esperandoNumeroDaMusica && k >= '1' && k <= '9') {
    // Converte o caractere ('1') para um número (1)
    int musicIndex = k - '1';

    // Toca a música usando o índice
    play(randomMelodies[musicIndex], randomMelodiesSize[musicIndex], randomMelodiesTempo[musicIndex]);

    Serial.print("Tocando a melodia ID: ");
    Serial.println(musicIndex + 1);

    // Reseta o estado para esperar a próxima entrada
    esperandoNumeroDaMusica = false;
    resetEntrada();
    return;
  }

  switch (estadoAtual) {
    case STANDBY:
      if (k == 'A') { //A - Jogo de musicas
        Serial.println("Jogo iniciado!");
        Serial.println("Adivinhe qual o filme/serie em 5 rodadas");
        Serial.println("Pressione '*' para SIM ou '#' para NAO.");
        Serial.println("A cada 'NAO' voce perde uma vida. Voce possui 2 vidas");
        Serial.println("Use com sabedoria");
        Serial.println("Rodada 1.");
        rodadaAtual = 1;
        respostasNao = 0;
        musicIndex = random(9);  // Sorteia uma musica de 0 a 9
        play(randomMelodies[musicIndex], randomMelodiesSize[musicIndex], randomMelodiesTempo[musicIndex]);
        Serial.print("Tocando a melodia ID: ");
        Serial.println(musicIndex + 1);
        estadoAtual = ESPERANDO_RESPOSTA;
        Serial.println("Musica finalizada.");
        play(melody_bips, sizeof(melody_bips) / sizeof(melody_bips[0]), 180);
        Serial.println("Você conhece a musica?");
        Serial.println("Pressione '*' para SIM ou '#' para NAO.");
        resetEntrada();
      }
      break;

    case ESPERANDO_RESPOSTA:
      if (k == '*') {
        Serial.println("Voce clicou em SIM!");
        proximaRodada();
      } else if (k == '#') {
        Serial.println("Voce clicou em NAO!");
        respostasNao++;
        if (respostasNao >= 2) {
          estadoAtual = FIM_DE_JOGO;
          Serial.println("Voce clicou em NAO 2 vezes! Tocando a musica de Game Over.");
          // Corrigido para melody_game_over
          play(melody_game_over, sizeof(melody_game_over) / sizeof(melody_game_over[0]), 100);
          Serial.println("FIM DE JOGO!");
          resetEntrada();
        } else {
          proximaRodada();
        }
      }
      break;

    case FIM_DE_JOGO:
      // O jogo terminou, volta para o estado inicial para que 'A' possa iniciar um novo
      estadoAtual = STANDBY;
      break;
  }


  if (k == 'B') { //B - Escolha uma musica pelo id do teclado
    esperandoNumeroDaMusica = true;  // Muda o estado para esperar o número
    Serial.println("Tecla 'B' pressionada. Digite o numero da musica (1 ate 9):");
    return;  // Sai do loop para esperar a próxima tecla
  }
  if (k == 'C') { //C - Musica aleatoria
     Serial.println("Musica aleatoria selecionada!");
    int musicIndex = random(NUM_MELODIES);
    play(randomMelodies[musicIndex], randomMelodiesSize[musicIndex], randomMelodiesTempo[musicIndex]);
    Serial.print("Tocando a melodia ID: ");
    Serial.println(musicIndex + 1);
    resetEntrada();
    return;
  }


  if ((k >= '0' && k <= '9') || (k >= 'A' && k <= 'D')) {
    if (lenEntrada < sizeof(entrada) - 1) {
      entrada[lenEntrada++] = k;
      entrada[lenEntrada] = '\0';
      //Serial.print("Buffer: '");
      Serial.print(entrada);
      Serial.println("'");
    } else {
      Serial.println("Buffer cheio -> reset");
      resetEntrada();
    }
  }
}

//FUNÇÃO PLAY()
void play(const int16_t* melody, size_t melody_len, int tempo) {
  const int events = melody_len / 2;
  const long wholenote = (60000L * 4) / tempo;

  for (int i = 0; i < events * 2; i += 2) {
    uint16_t freq = (uint16_t)pgm_read_word(&melody[i]);
    int16_t divider = (int16_t)pgm_read_word(&melody[i + 1]);

    long noteDuration;
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = wholenote / abs(divider);
      noteDuration = (noteDuration * 3) / 2;
    }

    if (freq == REST) {
      noTone(BUZZER_PIN);
      delay(noteDuration);
    } else {
      tone(BUZZER_PIN, freq, (noteDuration * 9) / 10);
      delay(noteDuration);
      noTone(BUZZER_PIN);
    }
  }
}