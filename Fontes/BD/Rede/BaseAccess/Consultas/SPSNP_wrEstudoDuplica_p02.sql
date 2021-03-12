  INSERT INTO
    Rede (intEstudoId, intRedeId, intTipoRedeId, intMRedeId, bolRedutivel, bolEstatica, bolRadial,
          bolEquivalente, dttRevisao, intColorId, tmsModificacao, strModificador,
          intBarraIniId, intEsqX1, intEsqY1, intEsqX2, intEsqY2, intUtmX1, intUtmY1, intUtmX2, intUtmY2,
          intAgrupamentoId
          )
  SELECT [p_EstudoIdNovo], intRedeId, intTipoRedeId, intMRedeId, bolRedutivel, bolEstatica, bolRadial,
        bolEquivalente, dttRevisao, intColorId, tmsModificacao, strModificador,
        intBarraIniId, intEsqX1, intEsqY1, intEsqX2, intEsqY2, intUtmX1, intUtmY1, intUtmX2, intUtmY2,
        intAgrupamentoId
  FROM Rede WHERE intEstudoId = [p_EstudoIdOrg];
