#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <openssl/x509.h>
#include <openssl/pem.h>

bool verifyCertificate(const char* caFile, const char* certFile)
{
    X509_STORE* store = X509_STORE_new();
    if (store == NULL) {
        fprintf(stderr, "Failed to create X509 store.\n");
        return false;
    }

    X509* caCert = NULL;
    FILE* caFilePtr = fopen(caFile, "r");
    if (caFilePtr == NULL) {
        fprintf(stderr, "Failed to open CA file: %s\n", caFile);
        X509_STORE_free(store);
        return false;
    }

    caCert = PEM_read_X509(caFilePtr, NULL, NULL, NULL);
    fclose(caFilePtr);
    if (caCert == NULL) {
        fprintf(stderr, "Failed to read CA certificate.\n");
        X509_STORE_free(store);
        return false;
    }

    if (X509_STORE_add_cert(store, caCert) != 1) {
        fprintf(stderr, "Failed to add CA certificate to store.\n");
        X509_free(caCert);
        X509_STORE_free(store);
        return false;
    }

    X509* cert = NULL;
    FILE* certFilePtr = fopen(certFile, "r");
    if (certFilePtr == NULL) {
        fprintf(stderr, "Failed to open certificate file: %s\n", certFile);
        X509_free(caCert);
        X509_STORE_free(store);
        return false;
    }

    cert = PEM_read_X509(certFilePtr, NULL, NULL, NULL);
    fclose(certFilePtr);
    if (cert == NULL) {
        fprintf(stderr, "Failed to read certificate.\n");
        X509_free(caCert);
        X509_STORE_free(store);
        return false;
    }

    X509_STORE_CTX* ctx = X509_STORE_CTX_new();
    if (ctx == NULL) {
        fprintf(stderr, "Failed to create X509 store context.\n");
        X509_free(cert);
        X509_free(caCert);
        X509_STORE_free(store);
        return false;
    }

    if (X509_STORE_CTX_init(ctx, store, cert, NULL) != 1) {
        fprintf(stderr, "Failed to initialize X509 store context.\n");
        X509_free(cert);
        X509_free(caCert);
        X509_STORE_free(store);
        X509_STORE_CTX_free(ctx);
        return false;
    }

    int result = X509_verify_cert(ctx);
    bool verified = (result == 1);

    X509_STORE_CTX_free(ctx);
    X509_free(cert);
    X509_free(caCert);
    X509_STORE_free(store);

    return verified;
}

int main()
{
    const char* caFile = "ca.pem";
    const char* certFile = "user.pem";
    bool result = verifyCertificate(caFile, certFile);

    if (result) {
        printf("Certificate verification succeeded.\n");
    } else {
        printf("Certificate verification failed.\n");
    }

    return 0;
}