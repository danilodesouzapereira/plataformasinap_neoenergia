SELECT
  intArvoreId     AS ArvoreId,
  strCodigo       AS Codigo,
  dttData         AS DataAtualizacao,
  intTipoArvoreId AS TipoArvoreId
FROM
  Arvore
ORDER BY
  dttData