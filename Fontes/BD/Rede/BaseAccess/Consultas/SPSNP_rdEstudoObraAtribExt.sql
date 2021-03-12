  SELECT
    ObraAtribExt.intAlternativaId  AS AlternativaId,
    ObraAtribExt.intObraId         AS ObraId,
    ObraAtribExt.intExtId          AS ExtId,
    ObraAtribExt.intAtribId        AS AtribId,
    ObraAtribExt.blbExt            AS BlobExt,
	ObraAtribExt.strDescr          AS Descr,
	ObraAtribExt.intItemId         AS ItemId
  FROM
    ObraAtribExt
  WHERE
    ObraAtribExt.intEstudoId = [p_EstudoId]
  ORDER BY
    ObraAtribExt.intAlternativaId,
    ObraAtribExt.intObraId;
