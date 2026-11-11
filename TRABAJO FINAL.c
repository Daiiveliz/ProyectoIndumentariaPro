#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* ============================================================================
   ESTRUCTURAS DE DATOS (struct)
   ============================================================================ */

typedef struct {
    int id;
    char dni[15];
    char nombre[50];
    char apellido[50];
    char telefono[20];
    char direccion[100];
    int rol; /* 1: Administrador, 2: Repositor, 3: Cliente */
    char usuario[30];
    char contrasena[30];
} Usuario;

typedef struct {
    int id;
    char nombre[30];
    float porcentajeRecargo;
} Categoria;

typedef struct {
    int id;
    char razonSocial[50];
    int origen; /* 1: Nacional, 2: Importado */
    int categorias[10];
    int cantidadCategorias;
    char usuario[30];
    char contrasena[30];
} Proveedor;

typedef struct {
    int id;
    char nombre[50];
    int idCategoria;
    int idProveedor;
    float costoCompra;
    float precioVenta;
    int stockS;
    int stockM;
    int stockL;
    int cantidadVendida;
} Prenda;

typedef struct {
    float cajaTotal;
    float cajaEfectivo;
    float cajaTarjeta;
    int totalFacturas;
} Caja;

typedef struct {
    int idPedido;
    int idProveedor;
    char fecha[30];
    int idProducto;
    char nombreProducto[50];
    char nombreCategoria[30];
    int cantidadS;
    int cantidadM;
    int cantidadL;
    float costoUnitario;
} PedidoProveedor;


/* ============================================================================
   FUNCIONES DE LIMPIZA Y UTILITARIAS
   ============================================================================ */

void limpiarPantalla(void) {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

void pausarLimpiar(void) {
    printf("\n  >> Presione ENTER para continuar...");
    getchar();
    getchar();
    limpiarPantalla();
}

void encabezadoSistema(char titulo[]) {
    limpiarPantalla();
    printf("\n====================================================================\n");
    printf("                    TIENDA PRO - %s\n", titulo);
    printf("====================================================================\n");
}


/* ============================================================================
   PERSISTENCIA EN ARCHIVOS DE TEXTO (.txt)
   ============================================================================ */

void guardarUsuarios(Usuario usuarios[], int cantidad) {
    FILE *archivo = fopen("usuarios.txt", "w");
    if (archivo == NULL) return;
    for (int i = 0; i < cantidad; i++) {
        fprintf(archivo, "%d|%s|%s|%s|%s|%s|%d|%s|%s\n",
                usuarios[i].id, usuarios[i].dni, usuarios[i].nombre, usuarios[i].apellido,
                usuarios[i].telefono, usuarios[i].direccion, usuarios[i].rol,
                usuarios[i].usuario, usuarios[i].contrasena);
    }
    fclose(archivo);
}

int cargarUsuarios(Usuario usuarios[], int maximo) {
    FILE *archivo = fopen("usuarios.txt", "r");
    int n = 0;
    if (archivo == NULL) return 0;
    while (n < maximo && fscanf(archivo, "%d|%14[^|]|%49[^|]|%49[^|]|%19[^|]|%99[^|]|%d|%29[^|]|%29[^\n]\n",
           &usuarios[n].id, usuarios[n].dni, usuarios[n].nombre, usuarios[n].apellido,
           usuarios[n].telefono, usuarios[n].direccion, &usuarios[n].rol,
           usuarios[n].usuario, usuarios[n].contrasena) == 9) {
        n++;
    }
    fclose(archivo);
    return n;
}

void guardarCategorias(Categoria categorias[], int cantidad) {
    FILE *archivo = fopen("categorias.txt", "w");
    if (archivo == NULL) return;
    for (int i = 0; i < cantidad; i++) {
        fprintf(archivo, "%d|%s|%.2f\n", categorias[i].id, categorias[i].nombre, categorias[i].porcentajeRecargo);
    }
    fclose(archivo);
}

int cargarCategorias(Categoria categorias[], int maximo) {
    FILE *archivo = fopen("categorias.txt", "r");
    int n = 0;
    if (archivo == NULL) return 0;
    while (n < maximo && fscanf(archivo, "%d|%29[^|]|%f\n",
           &categorias[n].id, categorias[n].nombre, &categorias[n].porcentajeRecargo) == 3) {
        n++;
    }
    fclose(archivo);
    return n;
}

void guardarProveedores(Proveedor proveedores[], int cantidad) {
    FILE *archivo = fopen("proveedores.txt", "w");
    if (archivo == NULL) return;
    for (int i = 0; i < cantidad; i++) {
        fprintf(archivo, "%d|%s|%d|%d", proveedores[i].id, proveedores[i].razonSocial, proveedores[i].origen, proveedores[i].cantidadCategorias);
        for (int j = 0; j < 10; j++) {
            fprintf(archivo, "|%d", j < proveedores[i].cantidadCategorias ? proveedores[i].categorias[j] : 0);
        }
        fprintf(archivo, "|%s|%s\n", proveedores[i].usuario, proveedores[i].contrasena);
    }
    fclose(archivo);
}

int cargarProveedores(Proveedor proveedores[], int maximo) {
    FILE *archivo = fopen("proveedores.txt", "r");
    int n = 0;
    if (archivo == NULL) return 0;
    while (n < maximo && fscanf(archivo, "%d|%49[^|]|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%29[^|]|%29[^\n]\n",
           &proveedores[n].id, proveedores[n].razonSocial, &proveedores[n].origen, &proveedores[n].cantidadCategorias,
           &proveedores[n].categorias[0], &proveedores[n].categorias[1], &proveedores[n].categorias[2], &proveedores[n].categorias[3],
           &proveedores[n].categorias[4], &proveedores[n].categorias[5], &proveedores[n].categorias[6], &proveedores[n].categorias[7],
           &proveedores[n].categorias[8], &proveedores[n].categorias[9],
           proveedores[n].usuario, proveedores[n].contrasena) == 16) {
        n++;
    }
    fclose(archivo);
    return n;
}

void guardarPrendas(Prenda prendas[], int cantidad) {
    FILE *archivo = fopen("prendas.txt", "w");
    if (archivo == NULL) return;
    for (int i = 0; i < cantidad; i++) {
        fprintf(archivo, "%d|%s|%d|%d|%.2f|%.2f|%d|%d|%d|%d\n",
                prendas[i].id, prendas[i].nombre, prendas[i].idCategoria, prendas[i].idProveedor,
                prendas[i].costoCompra, prendas[i].precioVenta, prendas[i].stockS, prendas[i].stockM,
                prendas[i].stockL, prendas[i].cantidadVendida);
    }
    fclose(archivo);
}

int cargarPrendas(Prenda prendas[], int maximo) {
    FILE *archivo = fopen("prendas.txt", "r");
    int n = 0;
    if (archivo == NULL) return 0;
    while (n < maximo && fscanf(archivo, "%d|%49[^|]|%d|%d|%f|%f|%d|%d|%d|%d\n",
           &prendas[n].id, prendas[n].nombre, &prendas[n].idCategoria, &prendas[n].idProveedor,
           &prendas[n].costoCompra, &prendas[n].precioVenta, &prendas[n].stockS, &prendas[n].stockM,
           &prendas[n].stockL, &prendas[n].cantidadVendida) == 10) {
        n++;
    }
    fclose(archivo);
    return n;
}

void guardarCaja(Caja caja[]) {
    FILE *archivo = fopen("caja.txt", "w");
    if (archivo == NULL) return;
    fprintf(archivo, "%.2f|%.2f|%.2f|%d\n", caja[0].cajaTotal, caja[0].cajaEfectivo, caja[0].cajaTarjeta, caja[0].totalFacturas);
    fclose(archivo);
}

int cargarCaja(Caja caja[]) {
    FILE *archivo = fopen("caja.txt", "r");
    if (archivo == NULL) return 0;
    int ok = fscanf(archivo, "%f|%f|%f|%d\n", &caja[0].cajaTotal, &caja[0].cajaEfectivo, &caja[0].cajaTarjeta, &caja[0].totalFacturas);
    fclose(archivo);
    return ok == 4;
}

void guardarPedidos(PedidoProveedor pedidos[], int cantidad) {
    FILE *archivo = fopen("pedidos.txt", "w");
    if (archivo == NULL) return;
    for (int i = 0; i < cantidad; i++) {
        fprintf(archivo, "%d|%d|%s|%d|%s|%s|%d|%d|%d|%.2f\n",
                pedidos[i].idPedido, pedidos[i].idProveedor, pedidos[i].fecha,
                pedidos[i].idProducto, pedidos[i].nombreProducto, pedidos[i].nombreCategoria,
                pedidos[i].cantidadS, pedidos[i].cantidadM, pedidos[i].cantidadL,
                pedidos[i].costoUnitario);
    }
    fclose(archivo);
}

int cargarPedidos(PedidoProveedor pedidos[], int maximo) {
    FILE *archivo = fopen("pedidos.txt", "r");
    int n = 0;
    if (archivo == NULL) return 0;
    while (n < maximo && fscanf(archivo, "%d|%d|%29[^|]|%d|%49[^|]|%29[^|]|%d|%d|%d|%f\n",
           &pedidos[n].idPedido, &pedidos[n].idProveedor, pedidos[n].fecha,
           &pedidos[n].idProducto, pedidos[n].nombreProducto, pedidos[n].nombreCategoria,
           &pedidos[n].cantidadS, &pedidos[n].cantidadM, &pedidos[n].cantidadL,
           &pedidos[n].costoUnitario) == 10) {
        n++;
    }
    fclose(archivo);
    return n;
}

void guardarTodosLosDatos(Usuario usuarios[], int cantUsuarios, Categoria categorias[], int cantCategorias,
                          Proveedor proveedores[], int cantProv, Prenda prendas[], int cantPrendas, Caja caja[],
                          PedidoProveedor pedidos[], int cantPedidos) {
    guardarUsuarios(usuarios, cantUsuarios);
    guardarCategorias(categorias, cantCategorias);
    guardarProveedores(proveedores, cantProv);
    guardarPrendas(prendas, cantPrendas);
    guardarCaja(caja);
    guardarPedidos(pedidos, cantPedidos);
}

void registrarVentaArchivo(char tipoFac, char cliente[], int idProducto, char producto[],
                           char talle, int cantidad, float total, char medioPago[]) {
    FILE *archivo = fopen("ventas.txt", "a");
    if (archivo == NULL) return;
    fprintf(archivo, "%ld|%c|%s|P%d|%s|%c|%d|%.2f|%s\n", (long)time(NULL), tipoFac, cliente, idProducto, producto, talle, cantidad, total, medioPago);
    fclose(archivo);
}


/* ============================================================================
   BÚSQUEDAS Y AUXILIARES
   ============================================================================ */

int buscarCategoria(Categoria categorias[], int cantCategorias, int id) {
    for (int i = 0; i < cantCategorias; i++) {
        if (categorias[i].id == id) return i;
    }
    return -1;
}

int buscarProveedor(Proveedor proveedores[], int cantProv, int id) {
    for (int i = 0; i < cantProv; i++) {
        if (proveedores[i].id == id) return i;
    }
    return -1;
}

int buscarPrenda(Prenda prendas[], int cantPrendas, int id) {
    for (int i = 0; i < cantPrendas; i++) {
        if (prendas[i].id == id) return i;
    }
    return -1;
}

void mostrarCategorias(Categoria categorias[], int cantCategorias) {
    printf("\n  CATEGORIAS DISPONIBLES:\n");
    printf("  ------------------------------------------------------------------\n");
    for (int i = 0; i < cantCategorias; i++) {
        printf("  ID: C%d | %-25s | Recargo: %.2f%%\n",
               categorias[i].id, categorias[i].nombre, categorias[i].porcentajeRecargo);
    }
    printf("  ------------------------------------------------------------------\n");
}


/* ============================================================================
   CARGA INICIAL DE DATOS POR DEFECTO
   ============================================================================ */

void cargarDatosIniciales(
    Usuario usuarios[], int cantUsuarios[],
    Categoria categorias[], int cantCategorias[],
    Proveedor proveedores[], int cantProv[],
    Prenda prendas[], int cantPrendas[],
    PedidoProveedor pedidos[], int cantPedidos[]
) {
    usuarios[0] = (Usuario){1, "11111111", "Carlos", "Gomez", "11-4444-5555", "Av. Corrientes 100", 1, "admin", "admin123"};
    usuarios[1] = (Usuario){2, "22222222", "Laura", "Perez", "11-6666-7777", "Calle Falsa 123", 2, "laura", "repo123"};
    usuarios[2] = (Usuario){3, "44444444", "Maria", "Lopez", "11-2222-3333", "San Martin 450", 3, "maria", "cliente123"};
    cantUsuarios[0] = 3;

    categorias[0] = (Categoria){1, "Vestimenta superior", 20.0};
    categorias[1] = (Categoria){2, "Vestimenta inferior", 25.0};
    cantCategorias[0] = 2;

    proveedores[0].id = 1;
    strcpy(proveedores[0].razonSocial, "Textil Sur SRL");
    proveedores[0].origen = 1;
    proveedores[0].categorias[0] = 1; proveedores[0].categorias[1] = 2;
    proveedores[0].cantidadCategorias = 2;
    strcpy(proveedores[0].usuario, "prov1");
    strcpy(proveedores[0].contrasena, "textil123");

    proveedores[1].id = 2;
    strcpy(proveedores[1].razonSocial, "Importadora Global");
    proveedores[1].origen = 2;
    proveedores[1].categorias[0] = 1;
    proveedores[1].cantidadCategorias = 1;
    strcpy(proveedores[1].usuario, "prov2");
    strcpy(proveedores[1].contrasena, "import123");
    cantProv[0] = 2;

    prendas[0] = (Prenda){1, "Remera", 1, 1, 12500.0, 15000.0, 5, 5, 5, 3};
    prendas[1] = (Prenda){2, "Pantalon", 2, 1, 25000.0, 31250.0, 4, 8, 2, 6};
    prendas[2] = (Prenda){3, "Campera", 1, 2, 40000.0, 48000.0, 2, 4, 1, 1};

    cantPrendas[0] = 3;
    cantPedidos[0] = 0;
}


/* ============================================================================
   GESTIÓN DE USUARIOS Y AUTENTICACIÓN
   ============================================================================ */

void registrarUsuario(Usuario usuarios[], int cantUsuarios[]) {
    Usuario u;
    if (cantUsuarios[0] >= 100) {
        printf("\n  [!] Limite maximo de usuarios alcanzado.\n");
        pausarLimpiar();
        return;
    }
    encabezadoSistema("ALTA DE USUARIO");
    u.id = cantUsuarios[0] + 1;
    printf("  ID asignado: U%d\n", u.id);
    printf("  > DNI: "); scanf("%s", u.dni);
    printf("  > Nombre: "); scanf("%s", u.nombre);
    printf("  > Apellido: "); scanf("%s", u.apellido);
    printf("  > Telefono: "); scanf("%s", u.telefono);
    printf("  > Direccion: "); scanf(" %[^\n]", u.direccion);

    do {
        printf("\n  Seleccione el rol:\n    1. Administrador\n    2. Repositor\n    3. Cliente\n  > Opcion: ");
        scanf("%d", &u.rol);
        if (u.rol < 1 || u.rol > 3) printf("  [!] Opcion invalida.\n");
    } while (u.rol < 1 || u.rol > 3);

    printf("  > Nombre de usuario: "); scanf("%29s", u.usuario);
    printf("  > Contrasena: "); scanf("%29s", u.contrasena);

    usuarios[cantUsuarios[0]] = u;
    cantUsuarios[0]++;
    printf("\n  [OK] Usuario U%d registrado correctamente.\n", u.id);
    pausarLimpiar();
}

int autenticarUsuario(Usuario usuarios[], int cantUsuarios, int rolEsperado) {
    char nombreUsuario[30];
    char contrasena[30];

    printf("\n  Nombre de usuario: "); scanf("%29s", nombreUsuario);
    printf("  Contrasena: "); scanf("%29s", contrasena);

    for (int i = 0; i < cantUsuarios; i++) {
        if (strcmp(usuarios[i].usuario, nombreUsuario) == 0 &&
            strcmp(usuarios[i].contrasena, contrasena) == 0 &&
            usuarios[i].rol == rolEsperado) {
            printf("\n  [OK] Bienvenido/a %s %s (Usuario U%d).\n", usuarios[i].nombre, usuarios[i].apellido, usuarios[i].id);
            return i;
        }
    }
    printf("\n  [!] Usuario, contrasena o perfil incorrecto.\n");
    pausarLimpiar();
    return -1;
}

int registrarCliente(Usuario usuarios[], int cantUsuarios[]) {
    Usuario u;
    if (cantUsuarios[0] >= 100) {
        printf("\n  [!] Limite de usuarios alcanzado.\n");
        pausarLimpiar();
        return -1;
    }
    encabezadoSistema("REGISTRO DE CLIENTE");
    u.id = cantUsuarios[0] + 1;
    u.rol = 3;
    printf("  > ID de Cliente asignado: U%d\n", u.id);
    printf("  > DNI: "); scanf("%14s", u.dni);
    printf("  > Nombre: "); scanf("%49s", u.nombre);
    printf("  > Apellido: "); scanf("%49s", u.apellido);
    printf("  > Telefono: "); scanf("%19s", u.telefono);
    printf("  > Direccion: "); scanf(" %99[^\n]", u.direccion);
    printf("  > Crear nombre de usuario: "); scanf("%29s", u.usuario);
    printf("  > Crear contrasena: "); scanf("%29s", u.contrasena);

    usuarios[cantUsuarios[0]] = u;
    cantUsuarios[0]++;
    printf("\n  [OK] Cuenta de cliente U%d creada correctamente.\n", u.id);
    pausarLimpiar();
    return cantUsuarios[0] - 1;
}


/* ============================================================================
   REPORTE EN TABLA DE PROVEEDORES Y MERCADERÍA
   ============================================================================ */

void mostrarMatrizProveedoresMercaderia(
    Proveedor proveedores[], int cantProv,
    Prenda prendas[], int cantPrendas,
    Categoria categorias[], int cantCategorias
) {
    char nombreProveedor[50];
    char nombreCategoria[30];
    float porcentajeRecargo;

    encabezadoSistema("REPORTE DE PROVEEDORES Y MERCADERIA");
    printf("%-8s | %-7s | %-12s | %-20s | %-9s | %-9s | %-6s | %-4s | %-5s\n",
           "PROV_ID", "PROD_ID", "MERCADERIA", "CATEGORIA", "P.PROV", "P.VENTA", "%RECAR", "VEND", "STOCK");
    printf("---------------------------------------------------------------------------------------------------\n");

    if (cantPrendas == 0) {
        printf("  No hay registro de productos en el sistema.\n");
    } else {
        for (int i = 0; i < cantPrendas; i++) {
            int posProv = buscarProveedor(proveedores, cantProv, prendas[i].idProveedor);
            if (posProv != -1) sprintf(nombreProveedor, "PV%d", proveedores[posProv].id);
            else strcpy(nombreProveedor, "Sin Asignar");

            int posCat = buscarCategoria(categorias, cantCategorias, prendas[i].idCategoria);
            if (posCat != -1) {
                strcpy(nombreCategoria, categorias[posCat].nombre);
                porcentajeRecargo = categorias[posCat].porcentajeRecargo;
            } else {
                strcpy(nombreCategoria, "Sin Categoria");
                porcentajeRecargo = 0.0f;
            }

            int totalStock = prendas[i].stockS + prendas[i].stockM + prendas[i].stockL;

            printf("%-8.8s | P%-6d | %-12.12s | %-20.20s | $%8.2f | $%8.2f | %5.1f%% | %4d | %5d\n",
                   nombreProveedor, prendas[i].id, prendas[i].nombre, nombreCategoria,
                   prendas[i].costoCompra, prendas[i].precioVenta,
                   porcentajeRecargo, prendas[i].cantidadVendida, totalStock);
        }
    }
    printf("---------------------------------------------------------------------------------------------------\n");
    pausarLimpiar();
}


/* ============================================================================
   PRODUCTOS, CATEGORÍAS E INGRESO DE MERCADERÍA CON PEDIDO
   ============================================================================ */

void registrarPrenda(Prenda prendas[], int cantPrendas[], Categoria categorias[], int cantCategorias) {
    Prenda p;
    int idCategoria;

    if (cantPrendas[0] >= 100) {
        printf("\n  [!] Capacidad maxima alcanzada.\n");
        pausarLimpiar();
        return;
    }

    encabezadoSistema("NUEVO PRODUCTO");
    p.id = cantPrendas[0] + 1;
    printf("  > ID asignado automaticamente: P%d\n", p.id);

    printf("  > Nombre de mercaderia (Ej: Remera, Pantalon, Campera, Short, Bufanda): ");
    scanf(" %[^\n]", p.nombre);

    mostrarCategorias(categorias, cantCategorias);
    printf("  > Numero ID de categoria (ej. 1 para C1): ");
    scanf("%d", &idCategoria);

    int posCategoria = buscarCategoria(categorias, cantCategorias, idCategoria);
    if (posCategoria == -1) {
        printf("\n  [!] Categoria C%d inexistente.\n", idCategoria);
        pausarLimpiar();
        return;
    }

    p.idCategoria = idCategoria;
    p.idProveedor = 0;
    p.costoCompra = 0; p.precioVenta = 0;
    p.stockS = 0; p.stockM = 0; p.stockL = 0;
    p.cantidadVendida = 0;

    prendas[cantPrendas[0]] = p;
    cantPrendas[0]++;
    printf("\n  [OK] Producto creado con ID P%d.\n", p.id);
    pausarLimpiar();
}

void registrarCategoria(Categoria categorias[], int cantCategorias[]) {
    Categoria c;
    if (cantCategorias[0] >= 30) {
        printf("\n  [!] Limite de categorias alcanzado.\n");
        pausarLimpiar();
        return;
    }
    encabezadoSistema("NUEVA CATEGORIA");
    c.id = cantCategorias[0] + 1;
    printf("  ID asignado: C%d\n", c.id);
    printf("  > Nombre de la nueva categoria: ");
    scanf(" %[^\n]", c.nombre);
    do {
        printf("  > Porcentaje de recargo sobre costo: ");
        scanf("%f", &c.porcentajeRecargo);
        if (c.porcentajeRecargo < 0) printf("  [!] El porcentaje no puede ser negativo.\n");
    } while (c.porcentajeRecargo < 0);

    categorias[cantCategorias[0]] = c;
    cantCategorias[0]++;
    printf("\n  [OK] Categoria C%d registrada con exito.\n", c.id);
    pausarLimpiar();
}

void ingresarMercaderia(
    Prenda prendas[], int cantPrendas,
    Proveedor proveedores[], int cantProv,
    Categoria categorias[], int cantCategorias,
    PedidoProveedor pedidos[], int cantPedidos[]
) {
    int idProveedor, idPrenda;
    float costo, nuevoPrecio;
    int cantS, cantM, cantL;

    encabezadoSistema("INGRESO DE MERCADERIA Y ENVIO DE PEDIDO");
    if (cantProv == 0 || cantPrendas == 0) {
        printf("\n  [!] Debe existir al menos un proveedor y un producto.\n");
        pausarLimpiar();
        return;
    }

    printf("\n  PROVEEDORES DISPONIBLES:\n");
    for (int i = 0; i < cantProv; i++) {
        printf("    PV%d - %s\n", proveedores[i].id, proveedores[i].razonSocial);
    }

    printf("\n  > Numero ID del proveedor (ej. 1 para PV1): ");
    scanf("%d", &idProveedor);

    int posProveedor = buscarProveedor(proveedores, cantProv, idProveedor);
    if (posProveedor == -1) {
        printf("\n  [!] Proveedor PV%d no encontrado.\n", idProveedor);
        pausarLimpiar();
        return;
    }

    printf("\n  Ingrese numero ID del producto a reponer (ej. 2 para P2): ");
    scanf("%d", &idPrenda);

    int posPrenda = buscarPrenda(prendas, cantPrendas, idPrenda);
    if (posPrenda == -1) {
        printf("\n  [!] El producto P%d no existe.\n", idPrenda);
        pausarLimpiar();
        return;
    }

    int idCategoria = prendas[posPrenda].idCategoria;
    int posCat = buscarCategoria(categorias, cantCategorias, idCategoria);

    printf("\n  > Costo acordado con el proveedor: $");
    scanf("%f", &costo);

    printf("  > Cantidad talle S a pedir: "); scanf("%d", &cantS);
    printf("  > Cantidad talle M a pedir: "); scanf("%d", &cantM);
    printf("  > Cantidad talle L a pedir: "); scanf("%d", &cantL);

    prendas[posPrenda].idProveedor = idProveedor;
    prendas[posPrenda].costoCompra = costo;
    if (posCat != -1) {
        nuevoPrecio = costo * (1.0f + (categorias[posCat].porcentajeRecargo / 100.0f));
        prendas[posPrenda].precioVenta = nuevoPrecio;
    }

    prendas[posPrenda].stockS += cantS;
    prendas[posPrenda].stockM += cantM;
    prendas[posPrenda].stockL += cantL;

    if (cantPedidos[0] < 200) {
        PedidoProveedor nuevoPed;
        nuevoPed.idPedido = cantPedidos[0] + 1;
        nuevoPed.idProveedor = idProveedor;
        sprintf(nuevoPed.fecha, "%ld", (long)time(NULL));

        nuevoPed.idProducto = prendas[posPrenda].id;
        strcpy(nuevoPed.nombreProducto, prendas[posPrenda].nombre);
        if (posCat != -1) strcpy(nuevoPed.nombreCategoria, categorias[posCat].nombre);
        else strcpy(nuevoPed.nombreCategoria, "Sin Categoria");

        nuevoPed.cantidadS = cantS;
        nuevoPed.cantidadM = cantM;
        nuevoPed.cantidadL = cantL;
        nuevoPed.costoUnitario = costo;

        pedidos[cantPedidos[0]] = nuevoPed;
        cantPedidos[0]++;
        guardarPedidos(pedidos, cantPedidos[0]);
    }

    printf("\n  [OK] Stock actualizado y Pedido PD%d registrado para el proveedor PV%d.\n", cantPedidos[0], idProveedor);
    pausarLimpiar();
}


/* ============================================================================
   FUNCIONES DE STOCK, CLIENTES Y VENTAS
   ============================================================================ */

void mostrarStock(Prenda prendas[], int cantPrendas, Categoria categorias[], int cantCategorias) {
    encabezadoSistema("CATALOGO Y STOCK GENERAL");
    printf("%-7s | %-20s | %-20s | %10s | %5s | %5s | %5s\n", "Codigo", "Producto", "Categoria", "Precio", "S", "M", "L");
    printf("------------------------------------------------------------------------------------\n");
    for (int i = 0; i < cantPrendas; i++) {
        int posCat = buscarCategoria(categorias, cantCategorias, prendas[i].idCategoria);
        printf("P%-6d | %-20.20s | %-20.20s | $%9.2f | %5d | %5d | %5d\n",
               prendas[i].id, prendas[i].nombre,
               posCat != -1 ? categorias[posCat].nombre : "Sin Categoria",
               prendas[i].precioVenta, prendas[i].stockS, prendas[i].stockM, prendas[i].stockL);
    }
    printf("------------------------------------------------------------------------------------\n");
    pausarLimpiar();
}

void alertaStockCritico(Prenda prendas[], int cantPrendas) {
    int hayAlerta = 0;
    encabezadoSistema("ALERTAS DE STOCK CRITICO");
    for (int i = 0; i < cantPrendas; i++) {
        if (prendas[i].stockS < 3 || prendas[i].stockM < 3 || prendas[i].stockL < 3) {
            printf("  [!] Reposicion requerida para: %s (Codigo: P%d)\n", prendas[i].nombre, prendas[i].id);
            printf("      Stock -> S: %d | M: %d | L: %d\n", prendas[i].stockS, prendas[i].stockM, prendas[i].stockL);
            hayAlerta = 1;
        }
    }
    if (!hayAlerta) printf("  [OK] No hay productos con stock critico.\n");
    pausarLimpiar();
}

void realizarVenta(Prenda prendas[], int cantPrendas, Caja caja[]) {
    int idPrenda, stockDisp, cantidad, medioPago;
    char talle, cliente[50], tipoFac;
    float subtotal, totalFinal;

    encabezadoSistema("MODULO DE VENTAS");
    printf("  > Numero ID del producto (ej. 1 para P1): ");
    scanf("%d", &idPrenda);

    int pos = buscarPrenda(prendas, cantPrendas, idPrenda);
    if (pos == -1) {
        printf("\n  [!] Producto P%d no encontrado.\n", idPrenda);
        pausarLimpiar();
        return;
    }

    printf("  Producto: %s | Precio: $%.2f\n", prendas[pos].nombre, prendas[pos].precioVenta);
    printf("  > Talle (S/M/L): "); scanf(" %c", &talle);
    talle = (char)toupper((unsigned char)talle);

    if (talle == 'S') stockDisp = prendas[pos].stockS;
    else if (talle == 'M') stockDisp = prendas[pos].stockM;
    else if (talle == 'L') stockDisp = prendas[pos].stockL;
    else {
        printf("\n  [!] Talle invalido.\n");
        pausarLimpiar();
        return;
    }

    printf("  Stock disponible: %d\n  > Cantidad: ", stockDisp);
    scanf("%d", &cantidad);

    if (cantidad <= 0 || cantidad > stockDisp) {
        printf("\n  [!] Cantidad invalida o stock insuficiente.\n");
        pausarLimpiar();
        return;
    }

    printf("  > Nombre del cliente: "); scanf(" %[^\n]", cliente);
    printf("  > Tipo de comprobante (A/B/C): "); scanf(" %c", &tipoFac);
    printf("  Medio de pago:\n    1. Efectivo (10%% desc)\n    2. Tarjeta / Transferencia\n  > Opcion: ");
    scanf("%d", &medioPago);

    subtotal = prendas[pos].precioVenta * cantidad;
    totalFinal = subtotal;
    if (medioPago == 1) {
        totalFinal = subtotal * 0.90;
        caja[0].cajaEfectivo += totalFinal;
    } else {
        caja[0].cajaTarjeta += totalFinal;
    }

    if (talle == 'S') prendas[pos].stockS -= cantidad;
    else if (talle == 'M') prendas[pos].stockM -= cantidad;
    else prendas[pos].stockL -= cantidad;

    prendas[pos].cantidadVendida += cantidad;
    caja[0].cajaTotal += totalFinal;
    caja[0].totalFacturas++;

    registrarVentaArchivo(tipoFac, cliente, prendas[pos].id, prendas[pos].nombre, talle, cantidad, totalFinal, medioPago == 1 ? "Efectivo" : "Tarjeta");

    printf("\n  [OK] Venta registrada con exito. Total a cobrar: $%.2f\n", totalFinal);
    pausarLimpiar();
}


/* ============================================================================
   PANEL DE PROVEEDOR
   ============================================================================ */

void menuProveedor(Proveedor proveedores[], int cantProv, PedidoProveedor pedidos[], int cantPedidos) {
    char usuario[30], contrasena[30];
    int pos = -1, encontrados = 0;

    encabezadoSistema("ACCESO DE PROVEEDORES");
    printf("  > Nombre de usuario: "); scanf("%29s", usuario);
    printf("  > Contrasena: "); scanf("%29s", contrasena);

    for (int i = 0; i < cantProv; i++) {
        if (strcmp(proveedores[i].usuario, usuario) == 0 && strcmp(proveedores[i].contrasena, contrasena) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("\n  [!] Usuario o contrasena incorrectos.\n");
        pausarLimpiar();
        return;
    }

    encabezadoSistema("PANEL DE PROVEEDOR - PEDIDOS RECIBIDOS");
    printf("  Proveedor: %s (ID: PV%d)\n--------------------------------------------------------------------\n",
           proveedores[pos].razonSocial, proveedores[pos].id);
    printf("%-6s | %-18s | %-12s | %-5s | %-5s | %-5s | %-10s\n", "ID_PED", "CATEGORIA", "PRODUCTO", "TAL.S", "TAL.M", "TAL.L", "P.UNIT");
    printf("--------------------------------------------------------------------\n");

    for (int j = 0; j < cantPedidos; j++) {
        if (pedidos[j].idProveedor == proveedores[pos].id) {
            printf("PD%-4d | %-18.18s | P%-3d (%-8.8s) | %5d | %5d | %5d | $%9.2f\n",
                   pedidos[j].idPedido, pedidos[j].nombreCategoria, pedidos[j].idProducto, pedidos[j].nombreProducto,
                   pedidos[j].cantidadS, pedidos[j].cantidadM, pedidos[j].cantidadL, pedidos[j].costoUnitario);
            encontrados++;
        }
    }

    if (encontrados == 0) printf("  No tiene pedidos pendientes.\n");
    pausarLimpiar();
}


/* ============================================================================
   MENÚS POR ROL
   ============================================================================ */

void menuAdministrador(
    Usuario usuarios[], int cantUsuarios[], Categoria categorias[], int cantCategorias[],
    Proveedor proveedores[], int cantProv[], Prenda prendas[], int cantPrendas[],
    Caja caja[], PedidoProveedor pedidos[], int cantPedidos[]
) {
    int opcion;
    do {
        encabezadoSistema("PANEL ADMINISTRADOR");
        printf("  [1] Registrar nuevo usuario\n");
        printf("  [2] Alta de Categoria\n");
        printf("  [3] Alta de Producto\n");
        printf("  [4] Ingreso de Mercaderia (Genera Pedido)\n");
        printf("  [5] Ver Matriz Proveedores-Mercaderia\n");
        printf("  [6] Guardar Cambios y Salir\n");
        printf("\n  > Seleccione opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: registrarUsuario(usuarios, cantUsuarios); break;
            case 2: registrarCategoria(categorias, cantCategorias); break;
            case 3: registrarPrenda(prendas, cantPrendas, categorias, cantCategorias[0]); break;
            case 4: ingresarMercaderia(prendas, cantPrendas[0], proveedores, cantProv[0], categorias, cantCategorias[0], pedidos, cantPedidos); break;
            case 5: mostrarMatrizProveedoresMercaderia(proveedores, cantProv[0], prendas, cantPrendas[0], categorias, cantCategorias[0]); break;
            case 6:
                guardarTodosLosDatos(usuarios, cantUsuarios[0], categorias, cantCategorias[0], proveedores, cantProv[0], prendas, cantPrendas[0], caja, pedidos, cantPedidos[0]);
                printf("\n  [OK] Datos guardados correctamente.\n");
                pausarLimpiar();
                break;
            default: printf("\n  [!] Opcion invalida.\n"); pausarLimpiar();
        }
    } while (opcion != 6);
}

void menuRepositor(Prenda prendas[], int cantPrendas, Categoria categorias[], int cantCategorias, Proveedor proveedores[], int cantProv, PedidoProveedor pedidos[], int cantPedidos[]) {
    int opcion;
    do {
        encabezadoSistema("PANEL REPOSITOR / EMPLEADO");
        printf("  [1] Consultar stock actual\n");
        printf("  [2] Consultar alertas de stock critico\n");
        printf("  [3] Ingresar mercaderia (Genera Pedido)\n");
        printf("  [4] Volver al menu principal\n");
        printf("\n  > Seleccione opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: mostrarStock(prendas, cantPrendas, categorias, cantCategorias); break;
            case 2: alertaStockCritico(prendas, cantPrendas); break;
            case 3: ingresarMercaderia(prendas, cantPrendas, proveedores, cantProv, categorias, cantCategorias, pedidos, cantPedidos); break;
            case 4: limpiarPantalla(); break;
            default: printf("\n  [!] Opcion invalida.\n"); pausarLimpiar();
        }
    } while (opcion != 4);
}

void menuCliente(Prenda prendas[], int cantPrendas, Categoria categorias[], int cantCategorias, Caja caja[]) {
    int opcion;
    do {
        encabezadoSistema("PANEL CLIENTE");
        printf("  [1] Ver catalogo y stock\n");
        printf("  [2] Comprar producto\n");
        printf("  [3] Volver al menu principal\n");
        printf("\n  > Seleccione opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: mostrarStock(prendas, cantPrendas, categorias, cantCategorias); break;
            case 2: realizarVenta(prendas, cantPrendas, caja); break;
            case 3: limpiarPantalla(); break;
            default: printf("\n  [!] Opcion invalida.\n"); pausarLimpiar();
        }
    } while (opcion != 3);
}

void accesoCliente(Usuario usuarios[], int cantUsuarios[], Prenda prendas[], int cantPrendas, Categoria categorias[], int cantCategorias, Caja caja[]) {
    int opcion;
    encabezadoSistema("ACCESO CLIENTES");
    printf("  [1] Iniciar sesion\n  [2] Registrarme como cliente\n  [3] Volver\n  > Opcion: ");
    scanf("%d", &opcion);

    if (opcion == 1) {
        if (autenticarUsuario(usuarios, cantUsuarios[0], 3) != -1) {
            menuCliente(prendas, cantPrendas, categorias, cantCategorias, caja);
        }
    } else if (opcion == 2) {
        if (registrarCliente(usuarios, cantUsuarios) != -1) {
            menuCliente(prendas, cantPrendas, categorias, cantCategorias, caja);
        }
    }
}


/* ============================================================================
   FUNCIÓN PRINCIPAL (MAIN)
   ============================================================================ */

int main(void) {
    Usuario usuarios[100];
    Categoria categorias[30];
    Proveedor proveedores[50];
    Prenda prendas[100];
    PedidoProveedor pedidos[200];
    Caja caja[1] = {{0.0f, 0.0f, 0.0f, 0}};

    int cantUsuarios[1] = {0};
    int cantCategorias[1] = {0};
    int cantProv[1] = {0};
    int cantPrendas[1] = {0};
    int cantPedidos[1] = {0};

    cantUsuarios[0] = cargarUsuarios(usuarios, 100);
    cantCategorias[0] = cargarCategorias(categorias, 30);
    cantProv[0] = cargarProveedores(proveedores, 50);
    cantPrendas[0] = cargarPrendas(prendas, 100);
    cantPedidos[0] = cargarPedidos(pedidos, 200);
    cargarCaja(caja);

    if (cantCategorias[0] == 0 || cantPrendas[0] == 0 || cantUsuarios[0] == 0) {
        cargarDatosIniciales(usuarios, cantUsuarios, categorias, cantCategorias, proveedores, cantProv, prendas, cantPrendas, pedidos, cantPedidos);
        guardarTodosLosDatos(usuarios, cantUsuarios[0], categorias, cantCategorias[0], proveedores, cantProv[0], prendas, cantPrendas[0], caja, pedidos, cantPedidos[0]);
    }

    int opcionPrincipal;
    limpiarPantalla();
    do {
        encabezadoSistema("MENU PRINCIPAL");
        printf("  [1] ADMINISTRADOR\n");
        printf("  [2] EMPLEADO / REPOSITOR\n");
        printf("  [3] PROVEEDOR\n");
        printf("  [4] CLIENTE (Ingresar o Registrarse)\n");
        printf("  [5] SALIR DEL SISTEMA\n");
        printf("\n  > Seleccione una opcion: ");
        scanf("%d", &opcionPrincipal);

        switch (opcionPrincipal) {
            case 1:
                if (autenticarUsuario(usuarios, cantUsuarios[0], 1) != -1) {
                    menuAdministrador(usuarios, cantUsuarios, categorias, cantCategorias, proveedores, cantProv, prendas, cantPrendas, caja, pedidos, cantPedidos);
                }
                limpiarPantalla();
                break;
            case 2:
                if (autenticarUsuario(usuarios, cantUsuarios[0], 2) != -1) {
                    menuRepositor(prendas, cantPrendas[0], categorias, cantCategorias[0], proveedores, cantProv[0], pedidos, cantPedidos);
                }
                limpiarPantalla();
                break;
            case 3:
                menuProveedor(proveedores, cantProv[0], pedidos, cantPedidos[0]);
                limpiarPantalla();
                break;
            case 4:
                accesoCliente(usuarios, cantUsuarios, prendas, cantPrendas[0], categorias, cantCategorias[0], caja);
                limpiarPantalla();
                break;
            case 5:
                printf("\n  [OK] Saliendo del sistema...\n");
                break;
            default:
                printf("\n  [!] Opcion invalida.\n");
                pausarLimpiar();
        }

        guardarTodosLosDatos(usuarios, cantUsuarios[0], categorias, cantCategorias[0], proveedores, cantProv[0], prendas, cantPrendas[0], caja, pedidos, cantPedidos[0]);
    } while (opcionPrincipal != 5);

    return 0;
}
