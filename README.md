### ULTIMATE FRUIT NINJA ###

Esse jogo se baseia no jogo Fruit Ninja, porém, em uma versão 3D para suprir as necessidades do trabalho.
Um cozinheiro, portando uma faca está em uma cozinha, onde tem a possibilidade de se locomover no espaço. As frutas vão começar a surgir caindo ao lado de uma das paredes e ele deve arremessar a faca para cortá-las antes que caiam no chão. Quanto mais frutas cortadas, maior o score. Se deixar a fruta cair no chão, perde.

### CONTRIBUIÇÕES ###

Texturas e Objetos
- Renderização de objetos: Vitória Lentz (00301893)
- Texturas dos objetos: Vitória Lentz (00301893) e Tiago Flach
- Modelagem: Vitória Lentz (00301893)
Movimentos
- Movimento da camera: Vitória Lentz (00301893)
- Movimento do chef: Vitória Lentz (00301893)
- Movimento das frutas: Vitória Lentz (00301893)
- Movimento da faca: Vitória Lentz (00301893)
Colisão
- Não foi feito
Fix de bugs:
- Tiago Flach e Vitória Lentz (00301893)

Foi usado o chatGPT para ajudar na parte da lógica de spawn das frutas e do movimento do chef. Para ajudar na definição dos vetores de direção, rotação e coordenadas. Mas a ferramenta não gerou informação muito útil, já que os vetores estavam sendo calculados errado.

### PROCESSO DE DESENVOLVIMENTO ###

O projeto foi iniciado sob o código dos laboratórios, por isso, muito do código é idêntico. A partir dele, foi primeiro feito a modelagem dos objetos na tela.
Após acertar os lugares dos objetos e a textura, foi feito o movimento das frutas. Com isso, o código de score e de aumento de velocidade com o passar das fases foi criado.
Em seguida, o movimento de camera foi criado, juntando a camera look-at e a livre no código. Nessa parte, o chef ainda não tinha sido criado. Somente após a camera se mover, o chef foi colocado em cena.

![image](https://github.com/user-attachments/assets/32df3055-ee7a-4989-8a38-08e1650b937e)


Por ultimo, a faca foi colocada no jogo, fazendo um movimento rotacional vertical em direção a parede das frutas.
Alguns bugs foram corrigidos no final, como o problema de texturas das frutas, mas o chef continua com problemas na textura.

Tentamos implementar a colisão, mas não funcionou. Tentamos pegar o código da internet, só para ver se funcionaria com ele, mas não funcionou. Então não demos continuidade a essa parte.


![image](https://github.com/user-attachments/assets/0b06bd30-ca78-4a85-8a7e-c4ae57a0cf79)

### MANUAL ###

- Tecla up: chef vai pra frente
- Tecla down: chef vai pra trás
- Tecla left: chef vai pra esquerda
- Tecla right: chef vai pra direita
- Tecla c: muda a camera de segunda pessoa para a primeira pessoa e vice-versa
- Click no mouse (lado esquerdo): atira a faca

### COMPILAÇÃO ###

Criar um projeto no CodeBlocks e adicionar todos os arquivos do diretório nele.
Compilar no codeblocks (o arquivo principal é o main.cpp).
Rodar o main.exe criado.
 
