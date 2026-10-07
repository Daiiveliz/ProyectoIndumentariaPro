#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ============================================================================
   ESTRUCTURAS DE DATOS
   ============================================================================ */

typedef struct {
    int id;
    char dni[15];
    char nombre[50];
    char apellido[50];
    char telefono[20];
    char direccion[100];
    int rol; /* 1: Administrador, 2: Repositor, 3: Cliente */
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
} Proveedor;

typedef struct {
    char codigo[10];
    char nombre[50];
    int idCategoria;
    float costoCompra;
    float precioVenta;
    int stockS;
    int stockM;
    int stockL;
} Prenda;

typedef struct {
    float cajaTotal;
    float cajaEfectivo;
    float cajaTarjeta;
    int totalFacturas;
} Caja;


/* ============================================================================
   FUNCIONES GENERALES
   ============================================================================ */

void pausarLimpiar() {
    printf("\n  >> Presione ENTER para continuar...");
    getchar();
    getchar();
}

void encabezadoSistema() {
    system("cls || clear");

    printf("====================================================================\n");
    printf("                    INDUMENTARIA PRO                               \n");
    printf("              SISTEMA DE GESTION Y STOCK                            \n");
    printf("====================================================================\n");
}

int buscarCategoria(Categoria categorias[], int cantCategorias, int id) {
    int i;

    for (i = 0; i < cantCategorias; i++) {
        if (categorias[i].id == id) {
            return i;
        }
    }

    return -1;
}

int buscarProveedor(Proveedor proveedores[], int cantProv, int id) {
    int i;

    for (i = 0; i < cantProv; i++) {
        if (proveedores[i].id == id) {
            return i;
        }
    }

    return -1;
}

int buscarPrenda(Prenda prendas[], int cantPrendas, char codigo[]) {
    int i;

    for (i = 0; i < cantPrendas; i++) {
        if (strcmp(prendas[i].codigo, codigo) == 0) {
            return i;
        }
    }

    return -1;
}

void mostrarCategorias(Categoria categorias[], int cantCategorias) {
    int i;

    printf("\n  CATEGORIAS DISPONIBLES:\n");
    printf("  ------------------------------------------------------------------\n");

    for (i = 0; i < cantCategorias; i++) {
        printf("  ID: %d | %-20s | Recargo: %.2f%%\n",
               categorias[i].id,
               categorias[i].nombre,
               categorias[i].porcentajeRecargo);
    }

    printf("  ------------------------------------------------------------------\n");
}


/* ============================================================================
   DATOS INICIALES
   ============================================================================ */

void cargarDatosIniciales(
    Usuario usuarios[],
    int cantUsuarios[],
    Categoria categorias[],
    int cantCategorias[],
    Proveedor proveedores[],
    int cantProv[],
    Prenda prendas[],
    int cantPrendas[]
) {
    /* -------------------- USUARIOS -------------------- */

    usuarios[0] = (Usuario){
        1, "11111111", "Carlos", "Gomez",
        "11-4444-5555", "Av. Corrientes 100", 1
    };

    usuarios[1] = (Usuario){
        2, "22222222", "Laura", "Perez",
        "11-6666-7777", "Calle Falsa 123", 2
    };

    usuarios[2] = (Usuario){
        3, "44444444", "Maria", "Lopez",
        "11-2222-3333", "San Martin 450", 3
    };

    cantUsuarios[0] = 3;


    /* -------------------- CATEGORIAS -------------------- */

    categorias[0] = (Categoria){1, "Remeras", 20.0};
    categorias[1] = (Categoria){2, "Pantalones", 25.0};
    categorias[2] = (Categoria){3, "Camperas", 30.0};
    categorias[3] = (Categoria){4, "Sacos", 30.0};

    cantCategorias[0] = 4;


    /* -------------------- PROVEEDORES -------------------- */

    proveedores[0].id = 101;
    strcpy(proveedores[0].razonSocial, "Textil Sur SRL");
    proveedores[0].origen = 1;
    proveedores[0].categorias[0] = 1;
    proveedores[0].categorias[1] = 2;
    proveedores[0].cantidadCategorias = 2;

    proveedores[1].id = 102;
    strcpy(proveedores[1].razonSocial, "Importadora Global");
    proveedores[1].origen = 2;
    proveedores[1].categorias[0] = 3;
    proveedores[1].categorias[1] = 4;
    proveedores[1].cantidadCategorias = 2;

    cantProv[0] = 2;


    /* -------------------- PRENDAS -------------------- */

    strcpy(prendas[0].codigo, "RE01");
    strcpy(prendas[0].nombre, "Remera Oversize");
    prendas[0].idCategoria = 1;
    prendas[0].costoCompra = 12500.0;
    prendas[0].precioVenta = 15000.0;
    prendas[0].stockS = 2;
    prendas[0].stockM = 5;
    prendas[0].stockL = 10;

    strcpy(prendas[1].codigo, "PA01");
    strcpy(prendas[1].nombre, "Pantalon Cargo");
    prendas[1].idCategoria = 2;
    prendas[1].costoCompra = 25600.0;
    prendas[1].precioVenta = 32000.0;
    prendas[1].stockS = 8;
    prendas[1].stockM = 12;
    prendas[1].stockL = 4;

    cantPrendas[0] = 2;
}


/* ============================================================================
   USUARIOS
   ============================================================================ */

void registrarUsuario(Usuario usuarios[], int cantUsuarios[]) {
    Usuario u;

    if (cantUsuarios[0] >= 100) {
        printf("\n  [!] Limite maximo de usuarios alcanzado.\n");
        pausarLimpiar();
        return;
    }

    encabezadoSistema();

    printf("                    [ ALTA DE USUARIO ]\n");
    printf("--------------------------------------------------------------------\n");

    /*
       El ID es global y autoincremental.
       Ejemplo: 1, 2, 3, 4...
    */
    u.id = cantUsuarios[0] + 1;

    printf("  ID asignado: %d\n", u.id);

    printf("  > DNI: ");
    scanf("%s", u.dni);

    printf("  > Nombre: ");
    scanf("%s", u.nombre);

    printf("  > Apellido: ");
    scanf("%s", u.apellido);

    printf("  > Telefono: ");
    scanf("%s", u.telefono);

    printf("  > Direccion: ");
    scanf(" %[^\n]", u.direccion);

    do {
        printf("\n  Seleccione el perfil:\n");
        printf("    1. Administrador\n");
        printf("    2. Repositor\n");
        printf("    3. Cliente\n");
        printf("  > Opcion: ");
        scanf("%d", &u.rol);

        if (u.rol < 1 || u.rol > 3) {
            printf("  [!] Opcion invalida.\n");
        }

    } while (u.rol < 1 || u.rol > 3);

    usuarios[cantUsuarios[0]] = u;
    cantUsuarios[0]++;

    printf("\n  [OK] Usuario registrado correctamente.\n");

    pausarLimpiar();
}

void listarUsuarios(Usuario usuarios[], int cantUsuarios) {
    int i;

    encabezadoSistema();

    printf("                    [ LISTADO DE USUARIOS ]\n");
    printf("--------------------------------------------------------------------\n");

    for (i = 0; i < cantUsuarios; i++) {

        printf("\n  ID: %d\n", usuarios[i].id);
        printf("  Nombre: %s %s\n",
               usuarios[i].nombre,
               usuarios[i].apellido);
        printf("  DNI: %s\n", usuarios[i].dni);
        printf("  Telefono: %s\n", usuarios[i].telefono);

        switch (usuarios[i].rol) {
            case 1:
                printf("  Rol: Administrador\n");
                break;

            case 2:
                printf("  Rol: Repositor\n");
                break;

            case 3:
                printf("  Rol: Cliente\n");
                break;
        }

        printf("  ------------------------------------------------------------------\n");
    }

    pausarLimpiar();
}


/* ============================================================================
   CATEGORIAS
   ============================================================================ */

void registrarCategoria(Categoria categorias[], int cantCategorias[]) {
    Categoria c;

    if (cantCategorias[0] >= 30) {
        printf("\n  [!] Limite de categorias alcanzado.\n");
        pausarLimpiar();
        return;
    }

    encabezadoSistema();

    printf("                    [ NUEVA CATEGORIA ]\n");
    printf("--------------------------------------------------------------------\n");

    c.id = cantCategorias[0] + 1;

    printf("  ID asignado: %d\n", c.id);

    printf("  > Nombre de la categoria: ");
    scanf(" %[^\n]", c.nombre);

    do {
        printf("  > Porcentaje de recargo sobre el costo: ");
        scanf("%f", &c.porcentajeRecargo);

        if (c.porcentajeRecargo < 0) {
            printf("  [!] El porcentaje no puede ser negativo.\n");
        }

    } while (c.porcentajeRecargo < 0);

    categorias[cantCategorias[0]] = c;
    cantCategorias[0]++;

    printf("\n  [OK] Categoria registrada.\n");
    printf("  El precio de venta se calculara usando este recargo.\n");

    pausarLimpiar();
}

void modificarRecargoCategoria(
    Categoria categorias[],
    int cantCategorias
) {
    int id;
    int pos;
    float nuevoPorcentaje;

    encabezadoSistema();

    printf("                [ MODIFICAR RECARGO POR CATEGORIA ]\n");
    printf("--------------------------------------------------------------------\n");

    mostrarCategorias(categorias, cantCategorias);

    printf("\n  > Ingrese ID de categoria: ");
    scanf("%d", &id);

    pos = buscarCategoria(categorias, cantCategorias, id);

    if (pos == -1) {
        printf("\n  [!] Categoria no encontrada.\n");
        pausarLimpiar();
        return;
    }

    printf("\n  Categoria: %s\n", categorias[pos].nombre);
    printf("  Recargo actual: %.2f%%\n", categorias[pos].porcentajeRecargo);

    do {
        printf("  > Nuevo porcentaje: ");
        scanf("%f", &nuevoPorcentaje);

        if (nuevoPorcentaje < 0) {
            printf("  [!] El porcentaje no puede ser negativo.\n");
        }

    } while (nuevoPorcentaje < 0);

    categorias[pos].porcentajeRecargo = nuevoPorcentaje;

    printf("\n  [OK] Recargo actualizado.\n");
    printf("  IMPORTANTE: esto no modifica automaticamente el stock existente.\n");

    pausarLimpiar();
}


/* ============================================================================
   PROVEEDORES
   ============================================================================ */

void mostrarProveedores(
    Proveedor proveedores[],
    int cantProv,
    Categoria categorias[],
    int cantCategorias
) {
    int i;
    int j;
    int posCategoria;

    encabezadoSistema();

    printf("                    [ LISTADO DE PROVEEDORES ]\n");
    printf("--------------------------------------------------------------------\n");

    for (i = 0; i < cantProv; i++) {

        printf("\n  ID: %d\n", proveedores[i].id);
        printf("  Razon social: %s\n", proveedores[i].razonSocial);

        if (proveedores[i].origen == 1) {
            printf("  Origen: Nacional\n");
        } else {
            printf("  Origen: Importado\n");
        }

        printf("  Categorias que comercializa:\n");

        for (j = 0; j < proveedores[i].cantidadCategorias; j++) {

            posCategoria = buscarCategoria(
                categorias,
                cantCategorias,
                proveedores[i].categorias[j]
            );

            if (posCategoria != -1) {
                printf("     - %s\n", categorias[posCategoria].nombre);
            }
        }

        printf("  ------------------------------------------------------------------\n");
    }

    pausarLimpiar();
}

void registrarProveedor(
    Proveedor proveedores[],
    int cantProv[],
    Categoria categorias[],
    int cantCategorias
) {
    Proveedor p;
    int opcionOrigen;
    int idCategoria;
    int posCategoria;

    if (cantProv[0] >= 50) {
        printf("\n  [!] Limite maximo de proveedores alcanzado.\n");
        pausarLimpiar();
        return;
    }

    encabezadoSistema();

    printf("                    [ NUEVO PROVEEDOR ]\n");
    printf("--------------------------------------------------------------------\n");

    p.id = 100 + cantProv[0] + 1;

    printf("  ID asignado: %d\n", p.id);

    printf("  > Razon social / Empresa: ");
    scanf(" %[^\n]", p.razonSocial);

    do {
        printf("\n  Origen:\n");
        printf("    1. Nacional\n");
        printf("    2. Importado\n");
        printf("  > Opcion: ");
        scanf("%d", &opcionOrigen);

    } while (opcionOrigen < 1 || opcionOrigen > 2);

    p.origen = opcionOrigen;
    p.cantidadCategorias = 0;

    while (p.cantidadCategorias < 10) {

        encabezadoSistema();

        printf("Proveedor: %s\n", p.razonSocial);
        printf("Categorias asignadas: %d\n\n", p.cantidadCategorias);

        mostrarCategorias(categorias, cantCategorias);

        printf("\n  Ingrese ID de categoria.\n");
        printf("  Ingrese 0 para finalizar.\n");
        printf("  > ID: ");
        scanf("%d", &idCategoria);

        if (idCategoria == 0) {
            break;
        }

        posCategoria = buscarCategoria(
            categorias,
            cantCategorias,
            idCategoria
        );

        if (posCategoria == -1) {
            printf("\n  [!] La categoria no existe.\n");
            pausarLimpiar();
        } else {
            proveedores[cantProv[0]].categorias[p.cantidadCategorias]
                = idCategoria;

            p.categorias[p.cantidadCategorias] = idCategoria;
            p.cantidadCategorias++;

            printf("\n  [OK] Categoria agregada al proveedor.\n");
            pausarLimpiar();
        }
    }

    proveedores[cantProv[0]] = p;
    cantProv[0]++;

    printf("\n  [OK] Proveedor registrado correctamente.\n");

    pausarLimpiar();
}


/* ============================================================================
   PRODUCTOS
   ============================================================================ */

void registrarPrenda(
    Prenda prendas[],
    int cantPrendas[],
    Categoria categorias[],
    int cantCategorias
) {
    Prenda p;
    int idCategoria;
    int posCategoria;

    if (cantPrendas[0] >= 100) {
        printf("\n  [!] Capacidad maxima de productos alcanzada.\n");
        pausarLimpiar();
        return;
    }

    encabezadoSistema();

    printf("                    [ NUEVO PRODUCTO ]\n");
    printf("--------------------------------------------------------------------\n");

    printf("  > Codigo del producto: ");
    scanf("%s", p.codigo);

    if (buscarPrenda(prendas, cantPrendas[0], p.codigo) != -1) {
        printf("\n  [!] Ese codigo ya existe.\n");
        pausarLimpiar();
        return;
    }

    printf("  > Nombre / descripcion: ");
    scanf(" %[^\n]", p.nombre);

    mostrarCategorias(categorias, cantCategorias);

    printf("  > ID de categoria: ");
    scanf("%d", &idCategoria);

    posCategoria = buscarCategoria(
        categorias,
        cantCategorias,
        idCategoria
    );

    if (posCategoria == -1) {
        printf("\n  [!] Categoria inexistente.\n");
        pausarLimpiar();
        return;
    }

    p.idCategoria = idCategoria;

    /*
       El producto se crea sin stock.
       La mercaderia entra posteriormente mediante
       "Ingreso de mercaderia".
    */
    p.costoCompra = 0;
    p.precioVenta = 0;
    p.stockS = 0;
    p.stockM = 0;
    p.stockL = 0;

    prendas[cantPrendas[0]] = p;
    cantPrendas[0]++;

    printf("\n  [OK] Producto creado correctamente.\n");
    printf("  Ahora puede ingresar mercaderia para agregar stock y costo.\n");

    pausarLimpiar();
}


/* ============================================================================
   INGRESO DE MERCADERIA
   ============================================================================ */

void ingresarMercaderia(
    Prenda prendas[],
    int cantPrendas,
    Proveedor proveedores[],
    int cantProv,
    Categoria categorias[],
    int cantCategorias
) {
    int idProveedor;
    int posProveedor;

    char codigo[10];
    int posPrenda;

    int idCategoria;
    int categoriaPermitida;
    int i;

    float costo;
    float nuevoPrecio;

    int cantidadS;
    int cantidadM;
    int cantidadL;

    encabezadoSistema();

    printf("                    [ INGRESO DE MERCADERIA ]\n");
    printf("--------------------------------------------------------------------\n");

    if (cantProv == 0 || cantPrendas == 0) {
        printf("\n  [!] Debe existir al menos un proveedor y un producto.\n");
        pausarLimpiar();
        return;
    }

    /* Paso 1: seleccionar proveedor */

    printf("\n  PROVEEDORES DISPONIBLES:\n");

    for (i = 0; i < cantProv; i++) {
        printf("    %d - %s\n",
               proveedores[i].id,
               proveedores[i].razonSocial);
    }

    printf("\n  > ID del proveedor: ");
    scanf("%d", &idProveedor);

    posProveedor = buscarProveedor(
        proveedores,
        cantProv,
        idProveedor
    );

    if (posProveedor == -1) {
        printf("\n  [!] Proveedor no encontrado.\n");
        pausarLimpiar();
        return;
    }

    /* Paso 2: mostrar categorias del proveedor */

    printf("\n  Categorias que comercializa %s:\n",
           proveedores[posProveedor].razonSocial);

    for (i = 0; i < proveedores[posProveedor].cantidadCategorias; i++) {

        int posCategoria = buscarCategoria(
            categorias,
            cantCategorias,
            proveedores[posProveedor].categorias[i]
        );

        if (posCategoria != -1) {
            printf("    ID %d - %s (Recargo %.2f%%)\n",
                   categorias[posCategoria].id,
                   categorias[posCategoria].nombre,
                   categorias[posCategoria].porcentajeRecargo);
        }
    }

    /* Paso 3: seleccionar categoria */

    printf("\n  > ID de categoria: ");
    scanf("%d", &idCategoria);

    categoriaPermitida = 0;

    for (i = 0; i < proveedores[posProveedor].cantidadCategorias; i++) {

        if (proveedores[posProveedor].categorias[i] == idCategoria) {
            categoriaPermitida = 1;
            break;
        }
    }

    if (!categoriaPermitida) {
        printf("\n  [!] Ese proveedor no comercializa esa categoria.\n");
        pausarLimpiar();
        return;
    }

    /* Paso 4: seleccionar producto */

    printf("\n  PRODUCTOS DE ESA CATEGORIA:\n");

    for (i = 0; i < cantPrendas; i++) {

        if (prendas[i].idCategoria == idCategoria) {
            printf("    %s - %s\n",
                   prendas[i].codigo,
                   prendas[i].nombre);
        }
    }

    printf("\n  > Codigo del producto: ");
    scanf("%s", codigo);

    posPrenda = buscarPrenda(
        prendas,
        cantPrendas,
        codigo
    );

    if (posPrenda == -1) {
        printf("\n  [!] Producto no encontrado.\n");
        pausarLimpiar();
        return;
    }

    if (prendas[posPrenda].idCategoria != idCategoria) {
        printf("\n  [!] El producto no pertenece a la categoria seleccionada.\n");
        pausarLimpiar();
        return;
    }

    /* Paso 5: costo de compra */

    printf("\n  Producto: %s\n", prendas[posPrenda].nombre);

    do {
        printf("  > Costo unitario de compra: $");
        scanf("%f", &costo);

        if (costo <= 0) {
            printf("  [!] El costo debe ser positivo.\n");
        }

    } while (costo <= 0);

    /*
       El precio de venta se calcula automaticamente:
       precioVenta = costo + recargo.
    */

    int posCategoria = buscarCategoria(
        categorias,
        cantCategorias,
        idCategoria
    );

    nuevoPrecio =
        costo * (1 + categorias[posCategoria].porcentajeRecargo / 100);

    /* Paso 6: cantidades */

    do {
        printf("  > Cantidad talle S: ");
        scanf("%d", &cantidadS);

        if (cantidadS < 0) {
            printf("  [!] La cantidad no puede ser negativa.\n");
        }

    } while (cantidadS < 0);

    do {
        printf("  > Cantidad talle M: ");
        scanf("%d", &cantidadM);

        if (cantidadM < 0) {
            printf("  [!] La cantidad no puede ser negativa.\n");
        }

    } while (cantidadM < 0);

    do {
        printf("  > Cantidad talle L: ");
        scanf("%d", &cantidadL);

        if (cantidadL < 0) {
            printf("  [!] La cantidad no puede ser negativa.\n");
        }

    } while (cantidadL < 0);

    /* Actualizar producto */

    prendas[posPrenda].costoCompra = costo;
    prendas[posPrenda].precioVenta = nuevoPrecio;

    prendas[posPrenda].stockS += cantidadS;
    prendas[posPrenda].stockM += cantidadM;
    prendas[posPrenda].stockL += cantidadL;

    printf("\n  ------------------------------------------------------------------\n");
    printf("  [OK] MERCADERIA INGRESADA\n");
    printf("  Proveedor: %s\n", proveedores[posProveedor].razonSocial);
    printf("  Producto: %s\n", prendas[posPrenda].nombre);
    printf("  Costo de compra: $%.2f\n", costo);
    printf("  Recargo de categoria: %.2f%%\n",
           categorias[posCategoria].porcentajeRecargo);
    printf("  Nuevo precio de venta: $%.2f\n", nuevoPrecio);
    printf("  Stock actual: S=%d | M=%d | L=%d\n",
           prendas[posPrenda].stockS,
           prendas[posPrenda].stockM,
           prendas[posPrenda].stockL);
    printf("  ------------------------------------------------------------------\n");

    pausarLimpiar();
}


/* ============================================================================
   STOCK Y CONSULTAS
   ============================================================================ */

void mostrarStock(
    Prenda prendas[],
    int cantPrendas,
    Categoria categorias[],
    int cantCategorias
) {
    int i;
    int posCategoria;

    encabezadoSistema();

    printf("                    [ STOCK GENERAL ]\n");
    printf("--------------------------------------------------------------------\n");

    for (i = 0; i < cantPrendas; i++) {

        posCategoria = buscarCategoria(
            categorias,
            cantCategorias,
            prendas[i].idCategoria
        );

        printf("\n  Codigo: %s\n", prendas[i].codigo);
        printf("  Producto: %s\n", prendas[i].nombre);

        if (posCategoria != -1) {
            printf("  Categoria: %s\n", categorias[posCategoria].nombre);
        }

        printf("  Costo de compra: $%.2f\n", prendas[i].costoCompra);
        printf("  Precio de venta: $%.2f\n", prendas[i].precioVenta);

        printf("  Stock -> S: %d | M: %d | L: %d\n",
               prendas[i].stockS,
               prendas[i].stockM,
               prendas[i].stockL);

        printf("  ------------------------------------------------------------------\n");
    }

    pausarLimpiar();
}

void consultarRentabilidad(
    Prenda prendas[],
    int cantPrendas,
    Categoria categorias[],
    int cantCategorias
) {
    int i;
    int posCategoria;
    float ganancia;

    encabezadoSistema();

    printf("                [ COSTOS, PRECIOS Y RENTABILIDAD ]\n");
    printf("--------------------------------------------------------------------\n");

    for (i = 0; i < cantPrendas; i++) {

        posCategoria = buscarCategoria(
            categorias,
            cantCategorias,
            prendas[i].idCategoria
        );

        ganancia =
            prendas[i].precioVenta - prendas[i].costoCompra;

        printf("\n  Producto: %s (%s)\n",
               prendas[i].nombre,
               prendas[i].codigo);

        if (posCategoria != -1) {
            printf("  Categoria: %s\n",
                   categorias[posCategoria].nombre);

            printf("  Recargo aplicado: %.2f%%\n",
                   categorias[posCategoria].porcentajeRecargo);
        }

        printf("  Costo de compra: $%.2f\n",
               prendas[i].costoCompra);

        printf("  Precio de venta: $%.2f\n",
               prendas[i].precioVenta);

        printf("  Ganancia por unidad: $%.2f\n",
               ganancia);

        printf("  ------------------------------------------------------------------\n");
    }

    pausarLimpiar();
}

void alertaStockCritico(
    Prenda prendas[],
    int cantPrendas
) {
    int i;
    int hayAlerta = 0;

    encabezadoSistema();

    printf("                  [ ALERTAS DE STOCK CRITICO ]\n");
    printf("--------------------------------------------------------------------\n");

    for (i = 0; i < cantPrendas; i++) {

        if (prendas[i].stockS < 3 ||
            prendas[i].stockM < 3 ||
            prendas[i].stockL < 3) {

            printf("\n  [!] REPOSICION REQUERIDA\n");
            printf("  Producto: %s (%s)\n",
                   prendas[i].nombre,
                   prendas[i].codigo);

            printf("  Stock -> S: %d | M: %d | L: %d\n",
                   prendas[i].stockS,
                   prendas[i].stockM,
                   prendas[i].stockL);

            printf("  ------------------------------------------------------------------\n");

            hayAlerta = 1;
        }
    }

    if (!hayAlerta) {
        printf("\n  [OK] No hay productos con stock critico.\n");
    }

    pausarLimpiar();
}


/* ============================================================================
   VENTAS Y FACTURACION
   ============================================================================ */

void emitirFactura(
    char tipoFac,
    char cliente[],
    char producto[],
    char talle,
    int cantidad,
    float total,
    char medioPago[]
) {
    encabezadoSistema();

    printf("                         FACTURA %c\n", toupper(tipoFac));
    printf("--------------------------------------------------------------------\n");

    printf("  Cliente: %s\n", cliente);
    printf("  Producto: %s\n", producto);
    printf("  Talle: %c\n", toupper(talle));
    printf("  Cantidad: %d\n", cantidad);
    printf("  Medio de pago: %s\n", medioPago);

    printf("  ------------------------------------------------------------------\n");
    printf("  TOTAL: $%.2f\n", total);
    printf("  ------------------------------------------------------------------\n");

    printf("                    Gracias por su compra.\n");
}

void realizarVenta(
    Prenda prendas[],
    int cantPrendas,
    Caja caja[]
) {
    char codigo[10];
    char talle;
    char cliente[50];
    char tipoFac;
    char medioStr[50];

    int pos;
    int stockDisponible;
    int cantidad;
    int medioPago;

    float subtotal;
    float totalFinal;

    encabezadoSistema();

    printf("                       [ MODULO DE VENTAS ]\n");
    printf("--------------------------------------------------------------------\n");

    printf("  > Codigo del producto: ");
    scanf("%s", codigo);

    pos = buscarPrenda(
        prendas,
        cantPrendas,
        codigo
    );

    if (pos == -1) {
        printf("\n  [!] Producto no encontrado.\n");
        pausarLimpiar();
        return;
    }

    printf("\n  Producto: %s\n", prendas[pos].nombre);
    printf("  Precio de venta: $%.2f\n", prendas[pos].precioVenta);

    printf("\n  > Talle (S/M/L): ");
    scanf(" %c", &talle);

    talle = toupper(talle);

    if (talle == 'S') {
        stockDisponible = prendas[pos].stockS;
    } else if (talle == 'M') {
        stockDisponible = prendas[pos].stockM;
    } else if (talle == 'L') {
        stockDisponible = prendas[pos].stockL;
    } else {
        printf("\n  [!] Talle invalido.\n");
        pausarLimpiar();
        return;
    }

    printf("  Stock disponible: %d\n", stockDisponible);

    printf("  > Cantidad: ");
    scanf("%d", &cantidad);

    if (cantidad <= 0 || cantidad > stockDisponible) {
        printf("\n  [!] Cantidad invalida o stock insuficiente.\n");
        pausarLimpiar();
        return;
    }

    printf("  > Nombre del cliente: ");
    scanf(" %[^\n]", cliente);

    printf("  > Tipo de comprobante (A/B/C): ");
    scanf(" %c", &tipoFac);

    printf("\n  Medio de pago:\n");
    printf("    1. Efectivo - 10%% de descuento\n");
    printf("    2. Tarjeta / Transferencia\n");
    printf("  > Opcion: ");
    scanf("%d", &medioPago);

    if (medioPago != 1 && medioPago != 2) {
        printf("\n  [!] Medio de pago invalido.\n");
        pausarLimpiar();
        return;
    }

    subtotal = prendas[pos].precioVenta * cantidad;
    totalFinal = subtotal;

    if (medioPago == 1) {
        totalFinal = subtotal * 0.90;
        strcpy(medioStr, "Efectivo - 10% descuento");

        caja[0].cajaEfectivo += totalFinal;
    } else {
        strcpy(medioStr, "Tarjeta / Transferencia");

        caja[0].cajaTarjeta += totalFinal;
    }

    /* Descontar stock */

    if (talle == 'S') {
        prendas[pos].stockS -= cantidad;
    } else if (talle == 'M') {
        prendas[pos].stockM -= cantidad;
    } else {
        prendas[pos].stockL -= cantidad;
    }

    /* Actualizar caja */

    caja[0].cajaTotal += totalFinal;
    caja[0].totalFacturas++;

    emitirFactura(
        tipoFac,
        cliente,
        prendas[pos].nombre,
        talle,
        cantidad,
        totalFinal,
        medioStr
    );

    pausarLimpiar();
}


/* ============================================================================
   CAJA
   ============================================================================ */

void cierreCajaZ(Caja caja[]) {

    encabezadoSistema();

    printf("                    [ CIERRE DE CAJA Z ]\n");
    printf("--------------------------------------------------------------------\n");

    printf("  Facturas emitidas: %d\n",
           caja[0].totalFacturas);

    printf("  Efectivo: $%.2f\n",
           caja[0].cajaEfectivo);

    printf("  Tarjeta / Transferencia: $%.2f\n",
           caja[0].cajaTarjeta);

    printf("  ------------------------------------------------------------------\n");

    printf("  TOTAL RECAUDADO: $%.2f\n",
           caja[0].cajaTotal);

    printf("  ------------------------------------------------------------------\n");

    /*
       Se reinicia la caja luego del cierre.
    */

    caja[0].cajaTotal = 0;
    caja[0].cajaEfectivo = 0;
    caja[0].cajaTarjeta = 0;
    caja[0].totalFacturas = 0;

    printf("  [OK] Caja reiniciada para un nuevo turno.\n");

    pausarLimpiar();
}


/* ============================================================================
   MENU ADMINISTRADOR
   ============================================================================ */

void menuAdmin(
    Usuario usuarios[],
    int cantUsuarios[],
    Categoria categorias[],
    int cantCategorias[],
    Proveedor proveedores[],
    int cantProv[],
    Prenda prendas[],
    int cantPrendas[],
    Caja caja[]
) {
    int opc;

    do {

        encabezadoSistema();

        printf("                    [ PANEL ADMINISTRADOR ]\n");
        printf("--------------------------------------------------------------------\n");

        printf("  1. Registrar un nuevo usuario\n");
        printf("  2. Consultar usuarios registrados\n");
        printf("  3. Registrar una nueva categoria de productos\n");
        printf("  4. Modificar el porcentaje de recargo de una categoria\n");
        printf("  5. Registrar un nuevo proveedor\n");
        printf("  6. Crear un nuevo producto en el catalogo\n");
        printf("  7. Registrar ingreso de mercaderia y actualizar stock\n");
        printf("  8. Consultar costos, precios y margen de ganancia\n");
        printf("  9. Realizar cierre de caja del turno (Caja Z)\n");
        printf(" 10. Cerrar sesion y volver al menu principal\n");

        printf("--------------------------------------------------------------------\n");
        printf("  > Opcion: ");
        scanf("%d", &opc);

        switch (opc) {

            case 1:
                registrarUsuario(
                    usuarios,
                    cantUsuarios
                );
                break;

            case 2:
                listarUsuarios(
                    usuarios,
                    cantUsuarios[0]
                );
                break;

            case 3:
                registrarCategoria(
                    categorias,
                    cantCategorias
                );
                break;

            case 4:
                modificarRecargoCategoria(
                    categorias,
                    cantCategorias[0]
                );
                break;

            case 5:
                registrarProveedor(
                    proveedores,
                    cantProv,
                    categorias,
                    cantCategorias[0]
                );
                break;

            case 6:
                registrarPrenda(
                    prendas,
                    cantPrendas,
                    categorias,
                    cantCategorias[0]
                );
                break;

            case 7:
                ingresarMercaderia(
                    prendas,
                    cantPrendas[0],
                    proveedores,
                    cantProv[0],
                    categorias,
                    cantCategorias[0]
                );
                break;

            case 8:
                consultarRentabilidad(
                    prendas,
                    cantPrendas[0],
                    categorias,
                    cantCategorias[0]
                );
                break;

            case 9:
                cierreCajaZ(caja);
                break;

            case 10:
                break;

            default:
                printf("\n  [!] Opcion invalida.\n");
                pausarLimpiar();
        }

    } while (opc != 10);
}


/* ============================================================================
   MENU REPOSITOR
   ============================================================================ */

void menuRepositor(
    Prenda prendas[],
    int cantPrendas,
    Categoria categorias[],
    int cantCategorias,
    Proveedor proveedores[],
    int cantProv
) {
    int opc;

    do {

        encabezadoSistema();

        printf("                     [ PANEL REPOSITOR ]\n");
        printf("--------------------------------------------------------------------\n");

        printf("  1. Consultar el stock actual de todos los productos\n");
        printf("  2. Consultar alertas de productos con stock critico\n");
        printf("  3. Registrar ingreso de mercaderia y aumentar el stock\n");
        printf("  4. Volver al menu principal\n");

        printf("--------------------------------------------------------------------\n");
        printf("  > Opcion: ");
        scanf("%d", &opc);

        switch (opc) {

            case 1:
                mostrarStock(
                    prendas,
                    cantPrendas,
                    categorias,
                    cantCategorias
                );
                break;

            case 2:
                alertaStockCritico(
                    prendas,
                    cantPrendas
                );
                break;

            case 3:
                ingresarMercaderia(
                    prendas,
                    cantPrendas,
                    proveedores,
                    cantProv,
                    categorias,
                    cantCategorias
                );
                break;

            case 4:
                break;

            default:
                printf("\n  [!] Opcion invalida.\n");
                pausarLimpiar();
        }

    } while (opc != 4);
}


/* ============================================================================
   MENU CLIENTE
   ============================================================================ */

void menuCliente(
    Prenda prendas[],
    int cantPrendas,
    Categoria categorias[],
    int cantCategorias,
    Caja caja[]
) {
    int opc;

    do {

        encabezadoSistema();

        printf("                      [ PANEL CLIENTE ]\n");
        printf("--------------------------------------------------------------------\n");

        printf("  1. Ver catalogo de productos, stock y precios\n");
        printf("  2. Comprar un producto y emitir comprobante\n");
        printf("  3. Volver al menu principal\n");

        printf("--------------------------------------------------------------------\n");
        printf("  > Opcion: ");
        scanf("%d", &opc);

        switch (opc) {

            case 1:
                mostrarStock(
                    prendas,
                    cantPrendas,
                    categorias,
                    cantCategorias
                );
                break;

            case 2:
                realizarVenta(
                    prendas,
                    cantPrendas,
                    caja
                );
                break;

            case 3:
                break;

            default:
                printf("\n  [!] Opcion invalida.\n");
                pausarLimpiar();
        }

    } while (opc != 3);
}


/* ============================================================================
   MENU PROVEEDOR
   ============================================================================ */

void menuProveedor(
    Proveedor proveedores[],
    int cantProv,
    Categoria categorias[],
    int cantCategorias
) {
    int id;
    int pos;

    encabezadoSistema();

    printf("                     [ CONSULTA DEL PROVEEDOR ]\n");
    printf("--------------------------------------------------------------------\n");

    printf("  > Ingrese su ID de proveedor registrado: ");
    scanf("%d", &id);

    pos = buscarProveedor(
        proveedores,
        cantProv,
        id
    );

    if (pos == -1) {
        printf("\n  [!] Proveedor no encontrado.\n");
        pausarLimpiar();
        return;
    }

    printf("\n  Proveedor: %s\n",
           proveedores[pos].razonSocial);

    if (proveedores[pos].origen == 1) {
        printf("  Origen: Nacional\n");
    } else {
        printf("  Origen: Importado\n");
    }

    printf("\n  Categorias que puede comercializar:\n");

    int i;

    for (i = 0; i < proveedores[pos].cantidadCategorias; i++) {

        int posCategoria = buscarCategoria(
            categorias,
            cantCategorias,
            proveedores[pos].categorias[i]
        );

        if (posCategoria != -1) {
            printf("    - %s\n",
                   categorias[posCategoria].nombre);
        }
    }

    pausarLimpiar();
}


/* ============================================================================
   MAIN
   ============================================================================ */

int main() {

    Usuario usuarios[100];
    Categoria categorias[30];
    Proveedor proveedores[50];
    Prenda prendas[100];
    Caja caja[1];

    int cantUsuarios[1] = {0};
    int cantCategorias[1] = {0};
    int cantProv[1] = {0};
    int cantPrendas[1] = {0};

    /* Inicializar caja */

    caja[0].cajaTotal = 0;
    caja[0].cajaEfectivo = 0;
    caja[0].cajaTarjeta = 0;
    caja[0].totalFacturas = 0;

    /* Cargar datos iniciales */

    cargarDatosIniciales(
        usuarios,
        cantUsuarios,
        categorias,
        cantCategorias,
        proveedores,
        cantProv,
        prendas,
        cantPrendas
    );

    int opcion;

    do {

        encabezadoSistema();

        printf("                       [ MENU PRINCIPAL ]\n");
        printf("--------------------------------------------------------------------\n");

        printf("  1. Acceder al panel de ADMINISTRACION\n");
        printf("  2. Acceder al panel de CONTROL DE STOCK\n");
        printf("  3. Consultar informacion como PROVEEDOR\n");
        printf("  4. Acceder al CATALOGO Y MODULO DE VENTAS\n");
        printf("  5. Salir del sistema\n");

        printf("--------------------------------------------------------------------\n");
        printf("  > Opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {

            case 1:
                menuAdmin(
                    usuarios,
                    cantUsuarios,
                    categorias,
                    cantCategorias,
                    proveedores,
                    cantProv,
                    prendas,
                    cantPrendas,
                    caja
                );
                break;

            case 2:
                menuRepositor(
                    prendas,
                    cantPrendas[0],
                    categorias,
                    cantCategorias[0],
                    proveedores,
                    cantProv[0]
                );
                break;

            case 3:
                menuProveedor(
                    proveedores,
                    cantProv[0],
                    categorias,
                    cantCategorias[0]
                );
                break;

            case 4:
                menuCliente(
                    prendas,
                    cantPrendas[0],
                    categorias,
                    cantCategorias[0],
                    caja
                );
                break;

            case 5:
                encabezadoSistema();
                printf("\n  [OK] Sistema finalizado correctamente.\n\n");
                break;

            default:
                printf("\n  [!] Opcion invalida.\n");
                pausarLimpiar();
        }

    } while (opcion != 5);

    return 0;
}
