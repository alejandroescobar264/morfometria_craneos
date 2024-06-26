# Resultados

## Versión 2.0

### Muestras Procesadas

Se observan a continuación las muestras elegidas:

![Muestras procesadas](img/muestras_procesadas.png)

Se muestra a modo de ejemplo una de los modelos correspondiente a la muestra A-150, con y sin textura:

![Muestras procesadas](img/GIF/A-150_textura.gif)

![Muestras procesadas](img/GIF/A-150.gif)

Las demás pueden observarse a continuación:

![Muestras procesadas](img/GIF/Modelos_3D_Con_Textura.gif)

![Muestras procesadas](img/GIF/Modelos_3D_Sin_Textura.gif)

En el apartado de Modelos se pueden encontrar las muestras individuales.

## Análisis Morfométrico 3D

### Colocación de Landmarks 3D

Para la colocación de Landmarks se empleó el software libre 3D Slicer, particularmente la librería SliceMorph.

Se colocaron 24 landmarks craneales, 16 en el hueso nasal y 22 en la nuca.

![3D Slicer - SliceMorph](img/3d_slicer.png)

![Landmarks](img/landmarks.png)

Se empleó el complemento ALPACA para poder transferir los landmarks de una plantilla al resto de los modelos, ahorrando mucho tiempo de marcado manual.

Se muestran todo el conjunto de modelos con los landmarks transferidos.

![Landmarks](img/landmarks_todos.png)

Se observa que los modelos no se encuentran en el mismo sistema de coordenadas, sin embargo, el analisis morfométrico se independiza del tamaño y no es necesario llevarlos todos al mismo sistema coordenado.

### Análisis de Procrustes Generalizado (GPA)

Se realizó el analisis de procrustes, obteniendose los siguientes resultados:

![Procrustes](img/procrustes_1.png)

Se muestra el plot de la variación de los landmarks en todos los modelos:

![Procrustes](img/procrustes_2.png)

#### Análisis del Cráneo

Se muestran los resultados del análisis de procrustes para los landmarks del cráneo:

![Procrustes](img/procrustes_3.png)

#### Análisis de la Hueso Nasal

Se muestran los resultados del análisis de procrustes para los landmarks del hueso nasal:

![Procrustes](img/procrustes_4.png)

#### Análisis del Foramen Magnum

Se muestran los resultados del análisis de procrustes para los landmarks del foramen magnum:

![Procrustes](img/procrustes_5.png)

### Alineamiento Simple

Se realizó un alineamiento simple entra las muestras A-147 y A-150 para observar las diferencias en las estructuras:

![Alineamiento Simple](img/alineamiento_1.png)

![Alineamiento Simple](img/alineamiento_2.png)

![Alineamiento Simple](img/alineamiento_3.png)




