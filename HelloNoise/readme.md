# Geração Procedural de Texturas com Coherent Noise

## Tutorial libnoise

Tutoriais 2 a 8 =>
[Link aqui](https://libnoise.sourceforge.net/tutorials/index.html)

## Resultados

### Geração de mapas vizinhos

| | |
|----------|----------|
| ![Mapa 3](./map-3.bmp) | ![Mapa 4](./map-4.bmp) |
| ![Mapa 1](./map-1.bmp) | ![Mapa 2](./map-2.bmp) | 

### Explorando números diferentes de `oitavas`

o ruído Perlin é a soma de várias funções de ruído coerente com frequências cada vez maiores e amplitudes cada vez menores. Cada uma dessas funções é chamada de oitava, porque cada oitava tem o dobro da frequência da oitava anterior.

| Octave 1 | Octave 6 |
|----------|----------|
| ![Oitava 1](./octave-1.bmp) | ![Oitava 2](./octave-2.bmp) |

### Explorando números diferentes de `frequência`

A frequência determina quantas mudanças ocorrem ao longo de uma unidade de comprimento. Aumentar a frequência aumentará o número de feições do terreno (e também diminuirá o tamanho dessas feições) em um mapa de altura do terreno.

| Frequência 1 | Frequência 16 |
|----------|----------|
| ![Frequência 1](./frequency-1.bmp) | ![Frequência 2](./frequency-2.bmp) |

### Explorando números diferentes de `persistência`

O valor de `persistência` determina a rapidez com que as amplitudes caem para cada oitava sucessiva. Aumentar o valor de persistência criará um mapa de altura de terreno mais acidentado, enquanto diminuir o valor de persistência criará um mapa de altura mais suave.

| Persistência 0 | Persistência 0.5 | Persistência 1 |
|----------|----------|----------|
| ![Persistência 1](./persistence-1.bmp) | ![Persistência 2](./persistence-2.bmp) | ![Persistência 3](./persistence-3.bmp) |

### 2 diferentes mapeamentos de biomas

| Mapa | Terreno montanhoso | 
|----------|----------|
| ![Mapa](./gradient-1.bmp) | ![Terreno montanhoso](./gradient-2.bmp) 

## Rodando a aplicação

### Adicionar Diretórios de inclusão adicionais

![Diretórios inclusão adicionais](./setup/diretorios-inclusao-adicionais.png)

### Adicionar Diretórios de biblioteca adicionais

![Diretórios biblioteca adicionais](./setup/diretorios-biblioteca-adicionais.png)

### Adicionar Dependências adicionais

![Dependências adicionais](./setup/dependencias-adicionais.png)

### Adicionar o arquivo `noise.dll` na pasta `Debug`

![noise.dll](./setup/noise-dll.png)