// TIENDA_PRO - Adaptacion del programa original en C a PSeInt.
// Sin archivos: los datos se mantienen en memoria durante la ejecucion.
// Los vectores paralelos reemplazan las estructuras (struct) de C.
// FUNCIONES: devuelven la posicion de un elemento o 0 si no existe.
Funcion posicion <- BuscarProducto(codigos, cantidad, buscado)
    Definir posicion, indice Como Entero;
    posicion <- 0;
    Para indice <- 1 Hasta cantidad Hacer
        Si codigos[indice] = buscado Entonces
            posicion <- indice;
        FinSi
    FinPara
FinFuncion

Funcion posicion <- BuscarCategoria(ids, cantidad, buscado)
    Definir posicion, indice Como Entero;
    posicion <- 0;
    Para indice <- 1 Hasta cantidad Hacer
        Si ids[indice] = buscado Entonces
            posicion <- indice;
        FinSi
    FinPara
FinFuncion

Funcion posicion <- BuscarProveedor(ids, cantidad, buscado)
    Definir posicion, indice Como Entero;
    posicion <- 0;
    Para indice <- 1 Hasta cantidad Hacer
        Si ids[indice] = buscado Entonces
            posicion <- indice;
        FinSi
    FinPara
FinFuncion

Funcion valor <- CalcularPrecio(costo, porcentaje)
    Definir valor Como Real;
    valor <- costo * (1 + porcentaje / 100);
FinFuncion

Funcion total <- StockDisponible(stockS, stockM, stockL)
    Definir total Como Entero;
    total <- stockS + stockM + stockL;
FinFuncion

SubProceso MostrarProducto(codigo, nombre, precio, stockS, stockM, stockL)
    Escribir codigo, " - ", nombre, " | Precio: $", precio;
    Escribir "Stock S: ", stockS, " M: ", stockM, " L: ", stockL;
FinSubProceso

Algoritmo TIENDA_PRO
    Definir opcion, subop, opcionProducto, eleccion, i, j, k, pos, pos2, nUsuarios, nCategorias, nProv, nPrendas, nFacturas Como Entero;
    Definir rolElegido, origenElegido, idBuscado, idCategoria, idProveedor, cantS, cantM, cantL, cantidad, disponible, pago Como Entero;
    Definir tipoFactura, talle, codigoBuscado, usuarioIngresado, claveIngresada, nombreCliente, nombreNuevo, claveNueva, medio Como Cadena;
    Definir costo, recargo, subtotal, total, cajaTotal, cajaEfectivo, cajaTarjeta, ganancia Como Real;
    Definir autorizado, encontrado, permitido, repetido, seguir Como Logico;
    Definir uId, uRol, catId, provId, provOrigen, provCantCat, provCategorias, prCat, prS, prM, prL Como Entero;
    Definir uDni, uNombre, uApellido, uTelefono, uDireccion, uUsuario, uClave, catNombre, provNombre, provUsuario, provClave, prCodigo, prNombre Como Cadena;
    Definir catRecargo, prCosto, prPrecio Como Real;
    Dimension uId[100], uRol[100], uDni[100], uNombre[100], uApellido[100], uTelefono[100], uDireccion[100], uUsuario[100], uClave[100];
    Dimension catId[30], catNombre[30], catRecargo[30];
    Dimension provId[50], provOrigen[50], provCantCat[50], provCategorias[50,10], provNombre[50], provUsuario[50], provClave[50];
    Dimension prCodigo[100], prNombre[100], prCat[100], prCosto[100], prPrecio[100], prS[100], prM[100], prL[100];
    // DATOS INICIALES equivalentes a cargarDatosIniciales() en C.
    nUsuarios <- 3;
    uId[1] <- 1; uDni[1] <- "11111111"; uNombre[1] <- "Carlos"; uApellido[1] <- "Gomez"; uTelefono[1] <- "11-4444-5555"; uDireccion[1] <- "Av. Corrientes 100"; uRol[1] <- 1; uUsuario[1] <- "admin"; uClave[1] <- "admin123";
    uId[2] <- 2; uDni[2] <- "22222222"; uNombre[2] <- "Laura"; uApellido[2] <- "Perez"; uTelefono[2] <- "11-6666-7777"; uDireccion[2] <- "Calle Falsa 123"; uRol[2] <- 2; uUsuario[2] <- "laura"; uClave[2] <- "repo123";
    uId[3] <- 3; uDni[3] <- "44444444"; uNombre[3] <- "Maria"; uApellido[3] <- "Lopez"; uTelefono[3] <- "11-2222-3333"; uDireccion[3] <- "San Martin 450"; uRol[3] <- 3; uUsuario[3] <- "maria"; uClave[3] <- "cliente123";
    nCategorias <- 4;
    catId[1] <- 1; catNombre[1] <- "Remeras"; catRecargo[1] <- 20;
    catId[2] <- 2; catNombre[2] <- "Pantalones"; catRecargo[2] <- 25;
    catId[3] <- 3; catNombre[3] <- "Camperas"; catRecargo[3] <- 30;
    catId[4] <- 4; catNombre[4] <- "Sacos"; catRecargo[4] <- 30;
    nProv <- 2;
    provId[1] <- 101; provNombre[1] <- "Textil Sur SRL"; provOrigen[1] <- 1; provCantCat[1] <- 2; provCategorias[1,1] <- 1; provCategorias[1,2] <- 2; provUsuario[1] <- "prov101"; provClave[1] <- "textil123";
    provId[2] <- 102; provNombre[2] <- "Importadora Global"; provOrigen[2] <- 2; provCantCat[2] <- 2; provCategorias[2,1] <- 3; provCategorias[2,2] <- 4; provUsuario[2] <- "prov102"; provClave[2] <- "import123";
    nPrendas <- 2;
    prCodigo[1] <- "RE01"; prNombre[1] <- "Remera Oversize"; prCat[1] <- 1; prCosto[1] <- 12500; prPrecio[1] <- 15000; prS[1] <- 2; prM[1] <- 5; prL[1] <- 10;
    prCodigo[2] <- "PA01"; prNombre[2] <- "Pantalon Cargo"; prCat[2] <- 2; prCosto[2] <- 25600; prPrecio[2] <- 32000; prS[2] <- 8; prM[2] <- 12; prL[2] <- 4;
    cajaTotal <- 0; cajaEfectivo <- 0; cajaTarjeta <- 0; nFacturas <- 0;
    Repetir
        Limpiar Pantalla;
        Escribir "        +======================================================+";
        Escribir "        |                     TIENDA_PRO                       |";
        Escribir "        |             SISTEMA DE INDUMENTARIA                 |";
        Escribir "        +======================================================+";
        Escribir "        |                    MENU PRINCIPAL                    |";
        Escribir "        |                                                      |";
        Escribir "        |    [1] ADMINISTRADOR                                 |";
        Escribir "        |    [2] EMPLEADO / REPOSITOR                          |";
        Escribir "        |    [3] PROVEEDOR                                     |";
        Escribir "        |    [4] CLIENTE                                       |";
        Escribir "        |    [5] SALIR                                         |";
        Escribir "        +======================================================+";
        Escribir "        Seleccione una opcion: " Sin Saltar;
        Leer opcion;
        Segun opcion Hacer
            1,2:
                Limpiar Pantalla;
                Si opcion = 1 Entonces
                    Escribir "            === ACCESO ADMINISTRADOR ===";
                SiNo
                    Escribir "            === ACCESO REPOSITOR ===";
                FinSi
                Escribir "Usuario: " Sin Saltar; Leer usuarioIngresado;
                Escribir "Contrasena: " Sin Saltar; Leer claveIngresada;
                autorizado <- Falso;
                Para i <- 1 Hasta nUsuarios Hacer
                    Si uUsuario[i] = usuarioIngresado Y uClave[i] = claveIngresada Y uRol[i] = opcion Entonces
                        autorizado <- Verdadero;
                        Escribir "Bienvenido/a ", uNombre[i], " ", uApellido[i];
                    FinSi
                FinPara
                Si autorizado Entonces
                    Si opcion = 1 Entonces
                        Repetir
                            Limpiar Pantalla;
                            Escribir "        +======================================================+";
                            Escribir "        |                 PANEL ADMINISTRADOR                  |";
                            Escribir "        +======================================================+";
                            Escribir "        | [1] Registrar usuario                               |";
                            Escribir "        | [2] Consultar usuarios                              |";
                            Escribir "        | [3] Registrar categoria                             |";
                            Escribir "        | [4] Modificar recargo de categoria                  |";
                            Escribir "        | [5] Registrar proveedor                             |";
                            Escribir "        | [6] Gestion de productos                            |";
                            Escribir "        | [7] Ingresar mercaderia                             |";
                            Escribir "        | [8] Consultar costos y rentabilidad                 |";
                            Escribir "        | [9] Cierre de caja Z                                |";
                            Escribir "        | [10] Cerrar sesion                                  |";
                            Escribir "        +======================================================+";
                            Leer subop;
                            Segun subop Hacer
                                1:
                                    Si nUsuarios < 100 Entonces
                                        nUsuarios <- nUsuarios + 1;
                                        uId[nUsuarios] <- uId[nUsuarios-1] + 1;
                                        Escribir "DNI: " Sin Saltar; Leer uDni[nUsuarios];
                                        Escribir "Nombre: " Sin Saltar; Leer uNombre[nUsuarios];
                                        Escribir "Apellido: " Sin Saltar; Leer uApellido[nUsuarios];
                                        Escribir "Telefono: " Sin Saltar; Leer uTelefono[nUsuarios];
                                        Escribir "Direccion: " Sin Saltar; Leer uDireccion[nUsuarios];
                                        Repetir
                                            Escribir "Rol (1 Administrador, 2 Repositor, 3 Cliente): " Sin Saltar; Leer uRol[nUsuarios];
                                        Hasta Que uRol[nUsuarios]>=1 Y uRol[nUsuarios]<=3;
                                        Escribir "Usuario: " Sin Saltar; Leer uUsuario[nUsuarios];
                                        Escribir "Contrasena: " Sin Saltar; Leer uClave[nUsuarios];
                                        Escribir "[OK] Usuario registrado. ID: ", uId[nUsuarios];
                                    SiNo
                                        Escribir "Limite de usuarios alcanzado.";
                                    FinSi
                                2:
                                    Escribir "============ USUARIOS REGISTRADOS ============";
                                    Para i <- 1 Hasta nUsuarios Hacer
                                        Escribir "ID: ",uId[i]," | ",uNombre[i]," ",uApellido[i]," | DNI: ",uDni[i]," | Rol: ",uRol[i];
                                        Escribir "Telefono: ",uTelefono[i];
                                        Escribir "--------------------------------------------";
                                    FinPara
                                3:
                                    Si nCategorias < 30 Entonces
                                        nCategorias <- nCategorias + 1;
                                        catId[nCategorias] <- catId[nCategorias-1] + 1;
                                        Escribir "Nombre de categoria: " Sin Saltar; Leer catNombre[nCategorias];
                                        Repetir
                                            Escribir "Recargo (%): " Sin Saltar; Leer catRecargo[nCategorias];
                                        Hasta Que catRecargo[nCategorias] >= 0;
                                        Escribir "[OK] Categoria creada. ID: ",catId[nCategorias];
                                    SiNo
                                        Escribir "Limite de categorias alcanzado.";
                                    FinSi
                                4:
                                    Para i <- 1 Hasta nCategorias Hacer
                                        Escribir catId[i]," - ",catNombre[i]," (",catRecargo[i],"%)";
                                    FinPara
                                    Escribir "ID a modificar: " Sin Saltar; Leer idBuscado;
                                    pos <- 0;
                                    Para i <- 1 Hasta nCategorias Hacer
                                        Si catId[i] = idBuscado Entonces pos <- i; FinSi
                                    FinPara
                                    Si pos > 0 Entonces
                                        Repetir
                                            Escribir "Nuevo recargo (%): " Sin Saltar; Leer recargo;
                                        Hasta Que recargo >= 0;
                                        catRecargo[pos] <- recargo;
                                        Escribir "[OK] Recargo actualizado. No cambia precios ya cargados.";
                                    SiNo
                                        Escribir "Categoria inexistente.";
                                    FinSi
                                5:
                                    Si nProv < 50 Entonces
                                        nProv <- nProv + 1;
                                        provId[nProv] <- provId[nProv-1] + 1;
                                        Escribir "Razon social: " Sin Saltar; Leer provNombre[nProv];
                                        Repetir
                                            Escribir "Origen (1 Nacional / 2 Importado): " Sin Saltar; Leer provOrigen[nProv];
                                        Hasta Que provOrigen[nProv]=1 O provOrigen[nProv]=2;
                                        provCantCat[nProv] <- 0;
                                        Repetir
                                            Para i <- 1 Hasta nCategorias Hacer
                                                Escribir catId[i]," - ",catNombre[i];
                                            FinPara
                                            Escribir "ID de categoria (0 para finalizar): " Sin Saltar; Leer idBuscado;
                                            Si idBuscado <> 0 Entonces
                                                encontrado <- Falso;
                                                Para i <- 1 Hasta nCategorias Hacer
                                                    Si catId[i]=idBuscado Entonces encontrado <- Verdadero; FinSi
                                                FinPara
                                                Si encontrado Y provCantCat[nProv] < 10 Entonces
                                                    provCantCat[nProv] <- provCantCat[nProv] + 1;
                                                    provCategorias[nProv,provCantCat[nProv]] <- idBuscado;
                                                    Escribir "Categoria agregada.";
                                                SiNo
                                                    Escribir "Categoria invalida o limite alcanzado.";
                                                FinSi
                                            FinSi
                                        Hasta Que idBuscado = 0 O provCantCat[nProv] = 10;
                                        Escribir "Usuario del proveedor: " Sin Saltar; Leer provUsuario[nProv];
                                        Escribir "Contrasena del proveedor: " Sin Saltar; Leer provClave[nProv];
                                        Escribir "[OK] Proveedor registrado. ID: ",provId[nProv];
                                    SiNo
                                        Escribir "Limite de proveedores alcanzado.";
                                    FinSi
                                6:
                                    Repetir
                                        Limpiar Pantalla;
                                        Escribir "        +======================================================+";
                                        Escribir "        |                 GESTION DE PRODUCTOS                 |";
                                        Escribir "        +======================================================+";
                                        Escribir "        | [1] Crear producto                                 |";
                                        Escribir "        | [2] Modificar producto                             |";
                                        Escribir "        | [3] Eliminar producto                              |";
                                        Escribir "        | [4] Buscar / consultar producto                    |";
                                        Escribir "        | [5] Volver al panel administrador                  |";
                                        Escribir "        +======================================================+";
                                        Escribir "        Seleccione una opcion: " Sin Saltar;
                                        Leer opcionProducto;
                                        Segun opcionProducto Hacer
                                1:
                                    Si nPrendas < 100 Entonces
                                        Escribir "Codigo de producto: " Sin Saltar; Leer codigoBuscado;
                                        repetido <- Falso;
                                        Para i <- 1 Hasta nPrendas Hacer
                                            Si prCodigo[i]=codigoBuscado Entonces repetido <- Verdadero; FinSi
                                        FinPara
                                        Si No repetido Entonces
                                            Escribir "Nombre / descripcion: " Sin Saltar; Leer nombreNuevo;
                                            Para i <- 1 Hasta nCategorias Hacer
                                                Escribir catId[i]," - ",catNombre[i];
                                            FinPara
                                            Escribir "ID de categoria: " Sin Saltar; Leer idBuscado;
                                            pos <- 0;
                                            Para i <- 1 Hasta nCategorias Hacer
                                                Si catId[i]=idBuscado Entonces pos <- i; FinSi
                                            FinPara
                                            Si pos > 0 Entonces
                                                nPrendas <- nPrendas + 1;
                                                prCodigo[nPrendas] <- codigoBuscado; prNombre[nPrendas] <- nombreNuevo; prCat[nPrendas] <- idBuscado;
                                                prCosto[nPrendas] <- 0; prPrecio[nPrendas] <- 0; prS[nPrendas] <- 0; prM[nPrendas] <- 0; prL[nPrendas] <- 0;
                                                Escribir "[OK] Producto creado. Ingrese mercaderia para cargar stock.";
                                            SiNo
                                                Escribir "Categoria inexistente.";
                                            FinSi
                                        SiNo
                                            Escribir "Ese codigo ya existe.";
                                        FinSi
                                    SiNo
                                        Escribir "Limite de productos alcanzado.";
                                    FinSi
                                2:
                                    Escribir "=== MODIFICAR PRODUCTO ===";
                                    Escribir "Codigo: " Sin Saltar; Leer codigoBuscado;
                                    pos <- BuscarProducto(prCodigo, nPrendas, codigoBuscado);
                                    Si pos > 0 Entonces
                                        Escribir "Nuevo nombre: " Sin Saltar; Leer nombreNuevo;
                                        Si nombreNuevo <> "" Entonces
                                            prNombre[pos] <- nombreNuevo;
                                            Escribir "[OK] Nombre actualizado.";
                                        SiNo
                                            Escribir "Nombre vacio: no se realizaron cambios.";
                                        FinSi
                                    SiNo
                                        Escribir "Producto no encontrado.";
                                    FinSi
                                3:
                                    Escribir "=== ELIMINAR PRODUCTO ===";
                                    Escribir "Codigo: " Sin Saltar; Leer codigoBuscado;
                                    pos <- BuscarProducto(prCodigo, nPrendas, codigoBuscado);
                                    Si pos > 0 Entonces
                                        Escribir "Confirma la baja? (1 SI / 2 NO): " Sin Saltar; Leer eleccion;
                                        Si eleccion = 1 Entonces
                                            Para i <- pos Hasta nPrendas - 1 Hacer
                                                prCodigo[i] <- prCodigo[i+1];
                                                prNombre[i] <- prNombre[i+1];
                                                prCat[i] <- prCat[i+1];
                                                prCosto[i] <- prCosto[i+1];
                                                prPrecio[i] <- prPrecio[i+1];
                                                prS[i] <- prS[i+1];
                                                prM[i] <- prM[i+1];
                                                prL[i] <- prL[i+1];
                                            FinPara
                                            nPrendas <- nPrendas - 1;
                                            Escribir "[OK] Producto eliminado.";
                                        SiNo
                                            Escribir "Operacion cancelada.";
                                        FinSi
                                    SiNo
                                        Escribir "Producto no encontrado.";
                                    FinSi
                                4:
                                    Escribir "=== CONSULTAR PRODUCTO ===";
                                    Escribir "Codigo: " Sin Saltar; Leer codigoBuscado;
                                    pos <- BuscarProducto(prCodigo, nPrendas, codigoBuscado);
                                    Si pos > 0 Entonces
                                        MostrarProducto(prCodigo[pos], prNombre[pos], prPrecio[pos], prS[pos], prM[pos], prL[pos]);
                                    SiNo
                                        Escribir "Producto no encontrado.";
                                    FinSi
                                5:
                                    Escribir "Volviendo al panel administrador...";
                                De Otro Modo:
                                    Escribir "Opcion invalida.";
                                        FinSegun
                                        Si opcionProducto <> 5 Entonces
                                            Escribir "Presione ENTER para continuar...";
                                            Esperar Tecla;
                                        FinSi
                                    Hasta Que opcionProducto = 5;
                                7:
                                    // Ingreso de mercaderia compartido con repositor (se desarrolla debajo).
                                    Escribir "=== INGRESO DE MERCADERIA ===";
                                    eleccion <- 1;
                                8:
                                    Escribir "======= COSTOS Y RENTABILIDAD =======";
                                    Para i <- 1 Hasta nPrendas Hacer
                                        ganancia <- prPrecio[i] - prCosto[i];
                                        Escribir prCodigo[i]," - ",prNombre[i]," | Costo: $",prCosto[i]," | Precio: $",prPrecio[i]," | Ganancia: $",ganancia;
                                    FinPara
                                9:
                                    Escribir "============== CIERRE Z ==============";
                                    Escribir "Facturas emitidas: ",nFacturas;
                                    Escribir "Efectivo: $",cajaEfectivo;
                                    Escribir "Tarjeta / Transferencia: $",cajaTarjeta;
                                    Escribir "TOTAL RECAUDADO: $",cajaTotal;
                                    cajaTotal <- 0; cajaEfectivo <- 0; cajaTarjeta <- 0; nFacturas <- 0;
                                    Escribir "[OK] Caja reiniciada para nuevo turno.";
                                10:
                                    Escribir "Sesion de administrador cerrada.";
                                De Otro Modo:
                                    Escribir "Opcion invalida.";
                            FinSegun
                            Si subop = 7 Entonces
                                // eleccion = 1 indica que se ingreso al modulo desde Administrador.
                                Si nProv>0 Y nPrendas>0 Entonces
                                    Para i <- 1 Hasta nProv Hacer
                                        Escribir provId[i]," - ",provNombre[i];
                                    FinPara
                                    Escribir "ID proveedor: " Sin Saltar; Leer idProveedor;
                                    pos <- 0;
                                    Para i <- 1 Hasta nProv Hacer
                                        Si provId[i]=idProveedor Entonces pos <- i; FinSi
                                    FinPara
                                    Si pos>0 Entonces
                                        Escribir "Categorias autorizadas:";
                                        Para j <- 1 Hasta provCantCat[pos] Hacer
                                            Escribir provCategorias[pos,j];
                                        FinPara
                                        Escribir "ID categoria: " Sin Saltar; Leer idCategoria;
                                        permitido <- Falso;
                                        Para j <- 1 Hasta provCantCat[pos] Hacer
                                            Si provCategorias[pos,j]=idCategoria Entonces permitido <- Verdadero; FinSi
                                        FinPara
                                        Si permitido Entonces
                                            Para i <- 1 Hasta nPrendas Hacer
                                                Si prCat[i]=idCategoria Entonces Escribir prCodigo[i]," - ",prNombre[i]; FinSi
                                            FinPara
                                            Escribir "Codigo producto: " Sin Saltar; Leer codigoBuscado;
                                            pos2 <- 0;
                                            Para i <- 1 Hasta nPrendas Hacer
                                                Si prCodigo[i]=codigoBuscado Y prCat[i]=idCategoria Entonces pos2 <- i; FinSi
                                            FinPara
                                            Si pos2>0 Entonces
                                                Repetir
                                                    Escribir "Costo unitario: $" Sin Saltar; Leer costo;
                                                Hasta Que costo>0;
                                                recargo <- 0;
                                                Para i <- 1 Hasta nCategorias Hacer
                                                    Si catId[i]=idCategoria Entonces recargo <- catRecargo[i]; FinSi
                                                FinPara
                                                Repetir
                                                    Escribir "Cantidad talle S: " Sin Saltar; Leer cantS;
                                                Hasta Que cantS>=0;
                                                Repetir
                                                    Escribir "Cantidad talle M: " Sin Saltar; Leer cantM;
                                                Hasta Que cantM>=0;
                                                Repetir
                                                    Escribir "Cantidad talle L: " Sin Saltar; Leer cantL;
                                                Hasta Que cantL>=0;
                                                prCosto[pos2] <- costo; prPrecio[pos2] <- CalcularPrecio(costo, recargo);
                                                prS[pos2] <- prS[pos2] + cantS; prM[pos2] <- prM[pos2] + cantM; prL[pos2] <- prL[pos2] + cantL;
                                                Escribir "[OK] Stock actualizado. Precio venta: $",prPrecio[pos2];
                                            SiNo
                                                Escribir "Producto inexistente en esa categoria.";
                                            FinSi
                                        SiNo
                                            Escribir "El proveedor no comercializa esa categoria.";
                                        FinSi
                                    SiNo
                                        Escribir "Proveedor no encontrado.";
                                    FinSi
                                SiNo
                                    Escribir "Debe existir un proveedor y un producto.";
                                FinSi
                            FinSi
                            Si subop <> 10 Entonces
                                Escribir "Presione ENTER para continuar...";
                                Esperar Tecla;
                            FinSi
                        Hasta Que subop = 10;
                    SiNo
                        Repetir
                            Limpiar Pantalla;
                            Escribir "        +======================================================+";
                            Escribir "        |                   PANEL REPOSITOR                    |";
                            Escribir "        +======================================================+";
                            Escribir "        | [1] Consultar stock                                 |";
                            Escribir "        | [2] Alertas de stock critico                        |";
                            Escribir "        | [3] Ingresar mercaderia                             |";
                            Escribir "        | [4] Cerrar sesion                                   |";
                            Escribir "        +======================================================+";
                            Leer subop;
                            Segun subop Hacer
                                1:
                                    Escribir "=============== STOCK ===============";
                                    Para i <- 1 Hasta nPrendas Hacer
                                        MostrarProducto(prCodigo[i], prNombre[i], prPrecio[i], prS[i], prM[i], prL[i]);
                                    FinPara
                                2:
                                    encontrado <- Falso;
                                    Para i <- 1 Hasta nPrendas Hacer
                                        Si prS[i]<3 O prM[i]<3 O prL[i]<3 Entonces
                                            Escribir "[!] REPOSICION: ",prNombre[i]," S=",prS[i]," M=",prM[i]," L=",prL[i];
                                            encontrado <- Verdadero;
                                        FinSi
                                    FinPara
                                    Si No encontrado Entonces Escribir "Sin stock critico."; FinSi
                                3:
                                    // El repositor puede ingresar mercaderia, igual que el administrador.
                                    Si nProv>0 Y nPrendas>0 Entonces
                                        Para i <- 1 Hasta nProv Hacer
                                            Escribir provId[i]," - ",provNombre[i];
                                        FinPara
                                        Escribir "ID proveedor: " Sin Saltar; Leer idProveedor;
                                        pos <- 0;
                                        Para i <- 1 Hasta nProv Hacer
                                            Si provId[i]=idProveedor Entonces pos <- i; FinSi
                                        FinPara
                                        Si pos>0 Entonces
                                            Escribir "Categorias del proveedor:";
                                            Para j <- 1 Hasta provCantCat[pos] Hacer
                                                Escribir provCategorias[pos,j];
                                            FinPara
                                            Escribir "ID categoria: " Sin Saltar; Leer idCategoria;
                                            permitido <- Falso;
                                            Para j <- 1 Hasta provCantCat[pos] Hacer
                                                Si provCategorias[pos,j]=idCategoria Entonces permitido <- Verdadero; FinSi
                                            FinPara
                                            Si permitido Entonces
                                                Para i <- 1 Hasta nPrendas Hacer
                                                    Si prCat[i]=idCategoria Entonces Escribir prCodigo[i]," - ",prNombre[i]; FinSi
                                                FinPara
                                                Escribir "Codigo producto: " Sin Saltar; Leer codigoBuscado;
                                                pos2 <- 0;
                                                Para i <- 1 Hasta nPrendas Hacer
                                                    Si prCodigo[i]=codigoBuscado Y prCat[i]=idCategoria Entonces pos2 <- i; FinSi
                                                FinPara
                                                Si pos2>0 Entonces
                                                    Repetir
                                                        Escribir "Costo unitario: $" Sin Saltar; Leer costo;
                                                    Hasta Que costo>0;
                                                    recargo <- 0;
                                                    Para i <- 1 Hasta nCategorias Hacer
                                                        Si catId[i]=idCategoria Entonces recargo <- catRecargo[i]; FinSi
                                                    FinPara
                                                    Repetir
                                                        Escribir "Cantidad S: " Sin Saltar; Leer cantS;
                                                    Hasta Que cantS>=0;
                                                    Repetir
                                                        Escribir "Cantidad M: " Sin Saltar; Leer cantM;
                                                    Hasta Que cantM>=0;
                                                    Repetir
                                                        Escribir "Cantidad L: " Sin Saltar; Leer cantL;
                                                    Hasta Que cantL>=0;
                                                    prCosto[pos2] <- costo; prPrecio[pos2] <- CalcularPrecio(costo, recargo);
                                                    prS[pos2] <- prS[pos2] + cantS; prM[pos2] <- prM[pos2] + cantM; prL[pos2] <- prL[pos2] + cantL;
                                                    Escribir "[OK] Mercaderia ingresada. Precio: $",prPrecio[pos2];
                                                SiNo
                                                    Escribir "Producto no encontrado.";
                                                FinSi
                                            SiNo
                                                Escribir "Categoria no permitida para proveedor.";
                                            FinSi
                                        SiNo
                                            Escribir "Proveedor no encontrado.";
                                        FinSi
                                    SiNo
                                        Escribir "No hay proveedores o productos.";
                                    FinSi
                                4:
                                    Escribir "Sesion cerrada.";
                                De Otro Modo:
                                    Escribir "Opcion invalida.";
                            FinSegun
                            Si subop<>4 Entonces Esperar Tecla; FinSi
                        Hasta Que subop=4;
                    FinSi
                SiNo
                    Escribir "[!] Usuario, contrasena o rol incorrectos.";
                    Esperar Tecla;
                FinSi
            3:
                Limpiar Pantalla;
                Escribir "=============== ACCESO PROVEEDOR ===============";
                Escribir "Usuario: " Sin Saltar; Leer usuarioIngresado;
                Escribir "Contrasena: " Sin Saltar; Leer claveIngresada;
                pos <- 0;
                Para i <- 1 Hasta nProv Hacer
                    Si provUsuario[i]=usuarioIngresado Y provClave[i]=claveIngresada Entonces pos <- i; FinSi
                FinPara
                Si pos>0 Entonces
                    Escribir "[OK] Proveedor: ",provNombre[pos]," | ID: ",provId[pos];
                    Si provOrigen[pos]=1 Entonces Escribir "Origen: Nacional"; SiNo Escribir "Origen: Importado"; FinSi
                    Escribir "Categorias que comercializa:";
                    Para j <- 1 Hasta provCantCat[pos] Hacer
                        Para i <- 1 Hasta nCategorias Hacer
                            Si catId[i]=provCategorias[pos,j] Entonces Escribir "- ",catNombre[i]; FinSi
                        FinPara
                    FinPara
                SiNo
                    Escribir "[!] Credenciales incorrectas.";
                FinSi
                Esperar Tecla;
            4:
                Repetir
                    Limpiar Pantalla;
                    Escribir "        +======================================================+";
                    Escribir "        |                   ACCESO CLIENTE                     |";
                    Escribir "        +======================================================+";
                    Escribir "        | [1] Iniciar sesion                                  |";
                    Escribir "        | [2] Registrarme                                     |";
                    Escribir "        | [3] Volver                                          |";
                    Escribir "        +======================================================+";
                    Leer eleccion;
                    autorizado <- Falso;
                    Segun eleccion Hacer
                        1:
                            Escribir "Usuario: " Sin Saltar; Leer usuarioIngresado;
                            Escribir "Contrasena: " Sin Saltar; Leer claveIngresada;
                            Para i <- 1 Hasta nUsuarios Hacer
                                Si uRol[i]=3 Y uUsuario[i]=usuarioIngresado Y uClave[i]=claveIngresada Entonces
                                    autorizado <- Verdadero;
                                    Escribir "Bienvenido/a ",uNombre[i];
                                FinSi
                            FinPara
                            Si No autorizado Entonces Escribir "Credenciales incorrectas."; Esperar Tecla; FinSi
                        2:
                            Si nUsuarios<100 Entonces
                                nUsuarios <- nUsuarios+1;
                                uId[nUsuarios] <- uId[nUsuarios-1]+1;
                                uRol[nUsuarios] <- 3;
                                Escribir "DNI: " Sin Saltar; Leer uDni[nUsuarios];
                                Escribir "Nombre: " Sin Saltar; Leer uNombre[nUsuarios];
                                Escribir "Apellido: " Sin Saltar; Leer uApellido[nUsuarios];
                                Escribir "Telefono: " Sin Saltar; Leer uTelefono[nUsuarios];
                                Escribir "Direccion: " Sin Saltar; Leer uDireccion[nUsuarios];
                                Repetir
                                    Escribir "Crear usuario: " Sin Saltar; Leer nombreNuevo;
                                    repetido <- Falso;
                                    Para i <- 1 Hasta nUsuarios-1 Hacer
                                        Si uUsuario[i]=nombreNuevo Entonces repetido <- Verdadero; FinSi
                                    FinPara
                                    Si repetido Entonces Escribir "Ese usuario ya existe."; FinSi
                                Hasta Que No repetido;
                                uUsuario[nUsuarios] <- nombreNuevo;
                                Escribir "Crear contrasena: " Sin Saltar; Leer uClave[nUsuarios];
                                autorizado <- Verdadero;
                                Escribir "[OK] Cliente registrado. ID: ",uId[nUsuarios];
                            SiNo
                                Escribir "Limite de usuarios alcanzado.";
                                Esperar Tecla;
                            FinSi
                        3:
                            Escribir "Volviendo al menu principal.";
                        De Otro Modo:
                            Escribir "Opcion invalida.";
                            Esperar Tecla;
                    FinSegun
                    Si autorizado Entonces
                        Repetir
                            Limpiar Pantalla;
                            Escribir "        +======================================================+";
                            Escribir "        |                    PANEL CLIENTE                     |";
                            Escribir "        +======================================================+";
                            Escribir "        | [1] Ver catalogo, stock y precios                   |";
                            Escribir "        | [2] Comprar y emitir comprobante                    |";
                            Escribir "        | [3] Cerrar sesion                                   |";
                            Escribir "        +======================================================+";
                            Leer subop;
                            Segun subop Hacer
                                1:
                                    Para i <- 1 Hasta nPrendas Hacer
                                        Escribir prCodigo[i]," - ",prNombre[i]," | $",prPrecio[i];
                                        Escribir "S: ",prS[i]," | M: ",prM[i]," | L: ",prL[i];
                                    FinPara
                                2:
                                    Escribir "Codigo del producto: " Sin Saltar; Leer codigoBuscado;
                                    pos <- 0;
                                    Para i <- 1 Hasta nPrendas Hacer
                                        Si prCodigo[i]=codigoBuscado Entonces pos <- i; FinSi
                                    FinPara
                                    Si pos>0 Entonces
                                        Escribir "Producto: ",prNombre[pos]," | Precio: $",prPrecio[pos];
                                        Escribir "Talle (S/M/L): " Sin Saltar; Leer talle;
                                        talle <- Mayusculas(talle);
                                        disponible <- -1;
                                        Segun talle Hacer
                                            "S": disponible <- prS[pos];
                                            "M": disponible <- prM[pos];
                                            "L": disponible <- prL[pos];
                                        FinSegun
                                        Si disponible>=0 Entonces
                                            Escribir "Stock disponible: ",disponible;
                                            Escribir "Cantidad: " Sin Saltar; Leer cantidad;
                                            Si cantidad>0 Y cantidad<=disponible Entonces
                                                Escribir "Nombre del cliente: " Sin Saltar; Leer nombreCliente;
                                                Escribir "Tipo comprobante (A/B/C): " Sin Saltar; Leer tipoFactura;
                                                Escribir "Medio de pago: 1 Efectivo (-10%) / 2 Tarjeta o transferencia";
                                                Leer pago;
                                                Si pago=1 O pago=2 Entonces
                                                    subtotal <- prPrecio[pos]*cantidad;
                                                    total <- subtotal;
                                                    Si pago=1 Entonces
                                                        total <- subtotal*0.90;
                                                        cajaEfectivo <- cajaEfectivo+total;
                                                        medio <- "Efectivo - 10% descuento";
                                                    SiNo
                                                        cajaTarjeta <- cajaTarjeta+total;
                                                        medio <- "Tarjeta / Transferencia";
                                                    FinSi
                                                    Segun talle Hacer
                                                        "S": prS[pos] <- prS[pos]-cantidad;
                                                        "M": prM[pos] <- prM[pos]-cantidad;
                                                        "L": prL[pos] <- prL[pos]-cantidad;
                                                    FinSegun
                                                    cajaTotal <- cajaTotal+total;
                                                    nFacturas <- nFacturas+1;
                                                    Limpiar Pantalla;
                                                    Escribir "        +======================================================+";
                                                    Escribir "        |                   FACTURA ",Mayusculas(tipoFactura),"                         |";
                                                    Escribir "        +======================================================+";
                                                    Escribir "Cliente: ",nombreCliente;
                                                    Escribir "Producto: ",prNombre[pos];
                                                    Escribir "Talle: ",talle," | Cantidad: ",cantidad;
                                                    Escribir "Pago: ",medio;
                                                    Escribir "TOTAL: $",total;
                                                    Escribir "Gracias por su compra.";
                                                SiNo
                                                    Escribir "Medio de pago invalido.";
                                                FinSi
                                            SiNo
                                                Escribir "Cantidad invalida o stock insuficiente.";
                                            FinSi
                                        SiNo
                                            Escribir "Talle invalido.";
                                        FinSi
                                    SiNo
                                        Escribir "Producto no encontrado.";
                                    FinSi
                                3:
                                    Escribir "Sesion cerrada.";
                                De Otro Modo:
                                    Escribir "Opcion invalida.";
                            FinSegun
                            Si subop<>3 Entonces Esperar Tecla; FinSi
                        Hasta Que subop=3;
                        eleccion <- 3;
                    FinSi
                Hasta Que eleccion=3;
            5:
                Escribir "Gracias por utilizar TIENDA_PRO.";
            De Otro Modo:
                Escribir "Opcion invalida.";
                Esperar Tecla;
        FinSegun
    Hasta Que opcion=5;
FinAlgoritmo
