# Charla: Data Science & Cloud Computing 

###### por Elastacloud

- Elastacloud: consultora especializada arquitectura cloud, data science, iot

  - desarrollo de ia y ml

  - Azure, power bi, databricks, SQL, R, scala, phyton

    

  ### Introducción a cloud engineering y bi

  - Problema: gran cantidad de datos, análisis imposible con herramientas tradicionales
  - El salto a la nube provee escalabilidad, distribuida, para trabajos redundantes y se puede pedir bajo demanda

- Plataformas: azure, google cloud, amazon ws

  - Vistas, máquina virtual, convenios...

- Big data: gran canitdad de datos generados. 

  - Volumen: En el año 2020 se podrían llegar a crear 40 ZB
  - Los datos no siempre llegan en la forma que queremos, llegan de formas muy distintas. Datos sanitarios, de redes sociales...
  - trabajo como ingeniero de datos: agrupar para su uso eficiente
  - velocidad: la nube es escalable, asíque la velocidad ya no es un límite
  - Veracidad de los datos: credibilidad, transparencia del procesado, vosibilidad

- Procesado de datos azure:

  - Entrada: datos que el cliente quiere procesar
  - /landing: perfil data engineer, data scientist. Azure data factory. "copia pega"
  - Databricks: procesamiento escalable distribuido. 
  - /staging: organización segín las necesidades del cliente
  - business inteligence: generar un modelo eficiente para que el cliente pueda ver los datos y tomar decisiones a partir de ello

- tecnología susceptible a cambio, ej paso de sql a azure synapse

- Uso de spark, y tecnologías open source

- Uso de metodologías de desarrollo ágil

  - sprints de 7 días, entre cada sprint se revisa y mejora
  - feedback del cliente en cada sprint

### Introducción a data science

- ciencia de datos auna estadistica, analsis de datos y aprendizaje maquina
  - extraer un valor y información adicional no obvia de los datos
- cientifico de datos: estadistica, mates + campo específico + tecnología, programación
- ciclo de vida de proyecto de ciencia de datos
  - Se encuentra entre el /landing y el /staging
    1. Entender el negocio: oportunidades, problemas...
    2. Enteder datos: datos disponibles, limitaciones, de pago, obtenibles? heterogeneidad
    3. Preparar los datos: tienen que encajar en los modelos de procesado, sin sesgos... se detectan problemas en el paso anterior
    4. Preparar el modelo, encaja con nuestroproblema?
    5. Evaluar, cumplen su cometido?
    6. Despliegue

### Caso de estudio: predecir respuesas a una campaña de marketing

Usamos una bd real libre

Camapña de marketing de un banco portugeus. llamadas a personas que ya tenemos su informaciṕn para intentar venderles un apquete. respuesta del cliente positiva o negativa

Antes de llamar al cliente: **quien va a responder más positivamente a la oferta?**

1. Preparar la bd, cargarla... 45mil entradas. Algunas de las cualidades no nos interesa meterlas a nuestro modelo: duration (duracion de la llamada) no existe pq estamos intentando predecirlo de forma previa a la llamada

2. exploración de nuestra bd. en este caso sabemos que está limpia, pero en la realidad habría que identificar problemas, ver la distribución de los datos... ej. grafico edad-balance de cuenta

3. test estadístico: edad vs suscripcion al servicio. Ej. a partir de los 65 es más probable que digan que sí, comparando las medianas de eddad y de respuesta

   > (pausa para trolleo indio)

4. Correlación de variables: queremos evitar redundancia, mantener el modelo simple y legible, muchas variables corremos riesgo de overfitting (el modelo se adapta tan bien a tu base de entrenamiento que no se adapta a la vida real)

5. Codificación de variables categóricas: variables dummy, codificacion ordinal para datos ordenaods

6. Normalizar las variables, reducir y optimizar la bd. Convertimos todas las variables en una distr normal

7. Dividir datos entre entrenamiento y test

   1. Entrenamiento: entrenar, 75% datos, darle al modelo las variables iniciales y finales y que infiera el procesamiento
   2. Test: 25%, lo contrario. Dado el proceso del modelo, comprobar que es correcto el resultado

8. Entrenar modelos. Hacer multiples modelos siempre

9. Comparar modelos con regresión logística. El modelo más simple puede ser facilmente la solución más eficiente

   1. Precisión: proporcion positivos correctos 
   2. Recall: proporción de positivos identificados

10. Balancear la poblaciónde entrenamiento

    1. Apuntamos a un 85% de precisión, más de eso es posible haber caído en overfitting



