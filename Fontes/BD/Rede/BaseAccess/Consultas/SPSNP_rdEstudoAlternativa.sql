  SELECT
    Estudo.intEstudoId                                                          AS EstudoId,
    Estudo.strDescricao                                                         AS EstudoDescricao,
    Estudo.strProprietario                                                      AS EstudoProprietario,
    Estudo.tmsModificacao                                                       AS EstudoModificacao,
	Estudo.strJustificativa                                                     AS EstudoJustificativa,
    EstudoAlternativa.intAlternativaId                                          AS AlternativaId,
    EstudoAlternativa.strDescricao                                              AS AlternativaDescricao,
	EstudoAlternativa.strJustificativa                                          AS AlternativaJustificativa,
    AlternativaObra.intObraId                                                   AS ObraId,
    AlternativaObra.strDescricao                                                AS ObraDescricao,
    AlternativaObra.dtaProjeto                                                  AS ObraDataProjeto,
    AlternativaObra.dtaExecucao                                                 AS ObraDataExecucao,
    AlternativaObra.strResponsavel                                              AS ObraResponsavel,
    AlternativaObra.decCusto                                                    AS ObraCusto,
    AlternativaObra.fltObrigEspecial                                            AS ObraObrigEspecial,
    AlternativaObra.fltDeltaCustoOM                                             AS DeltaCustoOM,
    AlternativaObra.bolAlteraTaxaFalha                                          AS bolAlteraTaxaFalha,
    AlternativaObra.fltTaxaFalhaIns                                             AS fltTaxaFalhaIns,
    AlternativaObra.fltTaxaFalhaSub                                             AS fltTaxaFalhaSub,
	AlternativaObra.bolFinalizada                                               AS bolFinalizada,
    AlternativaObra.bolBloqueada                                                AS bolBloqueada,	
    ObraAcao.intAcaoId                                                          AS AcaoId,
    ObraAcao.intTipoAcaoId                                                      AS TipoAcaoId,
	ObraAcao.intItemId                                                          AS ItemId,
    IIF(ISNULL(ObraAcao.intEquipOriginalId), -1, ObraAcao.intEquipOriginalId)   AS EquipOriginalId,
    IIF(ISNULL(ObraAcao.intEquipEstadoPreId), -1, ObraAcao.intEquipEstadoPreId) AS EquipEstadoPreId,
    IIF(ISNULL(ObraAcao.intEquipEstadoPosId), -1, ObraAcao.intEquipEstadoPosId) AS EquipEstadoPosId,
    Equipamento.intTipoEquipId                                                  AS TipoEquipId
  FROM
    (
    (
    (
    (
    Estudo
      INNER JOIN EstudoAlternativa ON
        Estudo.intEstudoId = EstudoAlternativa.intEstudoId
    )

      LEFT OUTER JOIN AlternativaObra ON
        EstudoAlternativa.intEstudoId = AlternativaObra.intEstudoId AND
        EstudoAlternativa.intAlternativaId = AlternativaObra.intAlternativaId
    )

      LEFT OUTER JOIN ObraAcao ON
        AlternativaObra.intEstudoId = ObraAcao.intEstudoId AND
        AlternativaObra.intAlternativaId = ObraAcao.intAlternativaId AND
        AlternativaObra.intObraId = ObraAcao.intObraId
    )

      LEFT OUTER JOIN Equipamento ON
        ObraAcao.intEstudoId = Equipamento.intEstudoId AND
        ObraAcao.intEquipOriginalId   = Equipamento.intEquipId
    )
  WHERE
    EstudoAlternativa.intEstudoId = [p_EstudoId]
  ORDER BY
    EstudoAlternativa.intAlternativaId,
    AlternativaObra.intObraId,
    ObraAcao.intAcaoId;
