# Controle de umidade – pendências (atualizado em 2026-11-10)

## Situação atual
- Circuito na protoboard funcionando: Arduino UNO (RoboCore) na **COM5**, DHT11 no pino 4, LED IR com 2N2222 no pino 3, VS1838B no pino 2 (só para captura).
- Ligar e desligar o ar por IR funciona com os sinais capturados do controle (`controle_umidade.ino`).
- Teste manual: o ar ligado em Dry baixou a água no ar de cerca de 13,6 para 10,7 g/m³ (25 °C/59 % para 22 °C/55 %).
- No fim do dia, o ar foi **desligado pelo controle**: o Arduino ainda acha que ele está ligado. **Apertar RESET na placa** para sincronizar.

## Próximos passos
1. ~~Recapturar o LIGAR~~ **Feito em 2026-11-14:** `CMD_LIGAR` agora é Dry, 22 °C, com o flap oscilando. Falta confirmar na prática se o compressor liga e se a umidade absoluta cai.
2. Acrescentar a **umidade absoluta (g/m³)** no monitor:
   `float absoluta = 6.112 * exp(17.67 * temp / (temp + 243.5)) * umidade * 2.1674 / (273.15 + temp);`
3. Avaliar mudar `UMIDADE_DESLIGA` de 45 para **50 %**, porque 45 % pode nunca ser alcançado.
4. Opcional: proteção contra frio (desligar o ar se a sala ficar abaixo de cerca de 19 °C).
5. Deixar rodando de 24 a 48 h com o script `log-umidade.ps1` e analisar o `umidade.csv`.

## Lembretes
- Parar o script do PowerShell (Ctrl+C) antes de gravar código no Arduino, porque ele ocupa a COM5.
- Captura: `RAW_BUFFER_LENGTH 400` e `RECORD_GAP_MICROS 20000` (a pausa inicial do Samsung é de cerca de 12,8 ms).
